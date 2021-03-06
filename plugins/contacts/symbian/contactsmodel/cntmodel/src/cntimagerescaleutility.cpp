/*
* Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
* Contact: http://www.qt-project.org/legal
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  
*
*/

#include "cntimagerescaler.h"
#include "cntimagerescaleutility.h"
#include "cntdbconsts_internal.h"

#include "cntitem.h"
#include "cntfldst.h"

#include <pathinfo.h>
#include <driveinfo.h>
#include <bautils.h>
#include <utf.h>

_LIT(KImagesBackupFolderName, "backup_registration_images.xml");
_LIT8(KXmlFilePart1,
            "<?xml version=\"1.0\" standalone=\"yes\"?>\r\n"
            "<backup_registration>\r\n"
            "    <public_backup delete_before_restore=\"yes\">\r\n"
            "        <include_directory name=\"");
_LIT8(KXmlFilePart2,
            "\"/>\r\n"
            "    </public_backup>\r\n"
            "</backup_registration>\r\n");
    
EXPORT_C TPath TCntImageRescaleUtility::ImageDirectoryL()
{
    TInt drive = KErrNotFound;
    RFs fs;
    CleanupClosePushL( fs );
    User::LeaveIfError( fs.Connect() );
        
    #ifdef __WINS__
        User::LeaveIfError(DriveInfo::GetDefaultDrive(DriveInfo::EDefaultPhoneMemory, drive));
    #else
        TInt errorCode = DriveInfo::GetDefaultDrive(DriveInfo::EDefaultMassStorage, drive);        
        if ( KErrNone == errorCode )
            {
            errorCode = TCntImageRescaleUtility::IsDriveReady( fs, drive );               
            }        
        User::LeaveIfError( errorCode );
    #endif
        
    // Get the root path in this drive to create the images directory
    TPath imagesDirPath;
    imagesDirPath.Zero();
    
    User::LeaveIfError(PathInfo::GetRootPath(imagesDirPath, drive));
    imagesDirPath.Append(KImagesFolder);
    
    if ( !BaflUtils::PathExists( fs, imagesDirPath ) )
        {
        BaflUtils::EnsurePathExistsL( fs, imagesDirPath );
        TCntImageRescaleUtility::CreateBackupAndRestoreFileL( fs, imagesDirPath );
        User::LeaveIfError(fs.SetAtt(imagesDirPath, KEntryAttHidden, KEntryAttNormal));
        }    
    
    CleanupStack::PopAndDestroy(); // RFs
    return imagesDirPath;
}

EXPORT_C TPath TCntImageRescaleUtility::CreateImageDirectoryL()
{
    TPath imagePath;
    TRAP_IGNORE( imagePath = TCntImageRescaleUtility::ImageDirectoryL() );    
    return imagePath;
}

EXPORT_C void TCntImageRescaleUtility::DeleteImageDirectoryL()
{
    TPath dir;
    TRAPD( err, dir = TCntImageRescaleUtility::ImageDirectoryL() );
    if ( err == KErrNone )
        {
        RFs fs;
        CleanupClosePushL( fs );
        User::LeaveIfError( fs.Connect() );
       
        CFileMan* fileMan = CFileMan::NewL( fs );
        fileMan->RmDir(dir); // err not used
        delete fileMan;
       
        CleanupStack::PopAndDestroy(); // close RFs
        }
}

EXPORT_C void TCntImageRescaleUtility::DeleteImageL( const CContactItem& aItem )
{
    CContactItemFieldSet& fieldSet = aItem.CardFields();
    RFs fs;
    CleanupClosePushL( fs );
    User::LeaveIfError( fs.Connect() );
                    
    // Find the image field
    TInt index = fieldSet.Find(KUidContactFieldCodImage, KUidContactFieldVCardMapUnknown);
    if (index != KErrNotFound)
        {
        // Image path field from list of contact fields
        CContactItemField& field = fieldSet[index];
        TPtrC imagePath = field.TextStorage()->Text();
                    
        // Remove image file if it is stored in private folder
        BaflUtils::DeleteFile(fs, imagePath); // Error value not necessary
        }
    
    CleanupStack::PopAndDestroy(); // RFs
}

EXPORT_C void TCntImageRescaleUtility::StoreImageFieldL( const CContactItem& aItem, const TPath aPath )
{
    CContactItemFieldSet& fieldSet = aItem.CardFields();
    if ( aPath.Length() > 0 )
    {
        CContactItemField* field = NULL;
                    
        TInt privateImageIndex = fieldSet.Find( KUidContactFieldCodImage, KUidContactFieldVCardMapUnknown );
        if ( privateImageIndex != KErrNotFound )
        {
            field = &fieldSet[privateImageIndex];
            field->TextStorage()->SetTextL( aPath );
        }
    }
}

EXPORT_C void TCntImageRescaleUtility::UpdateImageNameL( const CContactItem& aItem )
{
    CContactItemFieldSet& fieldSet = aItem.CardFields();
            
    // Find the image field
    TInt index = fieldSet.Find( KUidContactFieldCodImage, KUidContactFieldVCardMapUnknown );
    if (index != KErrNotFound)
    {
        // Image path field from list of contact fields
        CContactItemField& field = fieldSet[index];
        TPtrC privateImagePath = field.TextStorage()->Text();
        // unfortunately the Guid method is not const (?) and this ugly cast is needed
        TPtrC guid = const_cast<CContactItem&>(aItem).Guid();
                
        // Append the guid in the filename if it doesn't already exists
        if (privateImagePath.Length() && privateImagePath.Find(guid) == KErrNotFound)
        {
            // Image file type
            TParse p;
            p.Set(privateImagePath, NULL, NULL);
            
            // Generate the image path
            // Format <path>guid_timestamp_filename.ext
            TPath newImagePath;
            newImagePath.Append(TCntImageRescaleUtility::ImageDirectoryL());
            newImagePath.Append(guid);
            newImagePath.Append(p.NameAndExt());
            
            RFs fs;
            CleanupClosePushL( fs );
            User::LeaveIfError( fs.Connect() );
            User::LeaveIfError(BaflUtils::RenameFile(fs, privateImagePath, newImagePath)); // Rename the file
            CleanupStack::PopAndDestroy(); // RFs
            
            field.TextStorage()->SetTextL(newImagePath);
        }
    }
}
    
void TCntImageRescaleUtility::CreateBackupAndRestoreFileL( RFs& aFs, const TPath& aDir )
{
    TPath privatePath;
    User::LeaveIfError( aFs.PrivatePath(privatePath));
    User::LeaveIfError( aFs.SetSessionPath(privatePath));

    // If the path does not exist create it.
    TInt err = aFs.MkDirAll(privatePath);
    if (err != KErrAlreadyExists )
    {
        User::LeaveIfError(err);
    }

    // Remove previous backup registration file. The drive path may have
    // changed so this file needs to be regenerated all the time
    BaflUtils::DeleteFile( aFs, KImagesBackupFolderName()); // Error value not necessary

    const TInt newFileNameLength = KImagesBackupFolderName().Length();
    HBufC* newFileName = HBufC::NewLC(newFileNameLength);
    TPtr newFileNamePtr(newFileName->Des());
    newFileNamePtr.Append(KImagesBackupFolderName);

    // Create registration file.
    RFile file;
    CleanupClosePushL(file);

    // File should not exist since it was deleted
    User::LeaveIfError( file.Create(aFs, *newFileName, EFileWrite) );
            
    HBufC* nameAndExt = NULL;
    nameAndExt = HBufC::NewLC(aDir.Length());
    nameAndExt->Des().Append(aDir);

    // Convert foldername and extension to UTF8 before writing to file.
    HBufC8* folderName = HBufC8::NewLC(aDir.Length());
    TPtr8 pFolderName(folderName->Des());
        
    User::LeaveIfError(CnvUtfConverter::ConvertFromUnicodeToUtf8(pFolderName,*nameAndExt));

    // Write data into file.
    User::LeaveIfError(file.Write(KXmlFilePart1()));
    User::LeaveIfError(file.Write(pFolderName));
    User::LeaveIfError(file.Write(KXmlFilePart2()));
    User::LeaveIfError(file.Flush());

    User::LeaveIfError( aFs.SetSessionPath(aDir) );
    CleanupStack::PopAndDestroy(4, newFileName); // folderName, nameAndExt, file, newFileName
}

// ------------------------------------------------------------------------------------------------
// TInt IsDriveReady( RFs& aRfs, TInt aDrive )
// ------------------------------------------------------------------------------------------------
TInt TCntImageRescaleUtility::IsDriveReady( RFs& aRfs, TInt aDrive )
{  
    TUint driveStatus( 0 );    
    TInt ret( KErrNone );    
    TInt driveErr = DriveInfo::GetDriveStatus( aRfs, aDrive, driveStatus );
    
    if( KErrNone != driveErr ||
    !( driveStatus & DriveInfo::EDrivePresent ) ||
    ( driveStatus & DriveInfo::EDriveInUse ) )
    {
        ret = KErrBadHandle;           
    }

return ret;
}


// End of File
