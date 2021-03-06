/*
 * Copyright (c) 2002-2004,2011,2014 Apple Inc. All Rights Reserved.
 * 
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

/*!
	@header SecAccess
	SecAccess implements a way to set and manipulate access control rules and
	restrictions on SecKeychainItems.
*/

#ifndef _SECURITY_SECACCESS_H_
#define _SECURITY_SECACCESS_H_

#include <Security/SecBase.h>
#include <Security/cssmtype.h>
#include <CoreFoundation/CFArray.h>
#include <CoreFoundation/CFError.h>
#include <sys/types.h>
#include <unistd.h>


#if defined(__cplusplus)
extern "C" {
#endif



typedef UInt32	SecAccessOwnerType;
enum
{
	kSecUseOnlyUID = 1,
	kSecUseOnlyGID = 2,
	kSecHonorRoot = 0x100,
	kSecMatchBits = (kSecUseOnlyUID | kSecUseOnlyGID)
};

/* No restrictions. Permission to perform all operations on
   the resource or available to an ACL owner.  */
extern  CFTypeRef kSecACLAuthorizationAny
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);

extern CFTypeRef kSecACLAuthorizationLogin
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationGenKey
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationDelete
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationExportWrapped
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationExportClear
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationImportWrapped
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationImportClear
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationSign
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationEncrypt
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationDecrypt
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationMAC
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationDerive
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);

/* Defined authorization tag values for Keychain */
extern CFTypeRef kSecACLAuthorizationKeychainCreate
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationKeychainDelete
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationKeychainItemRead
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationKeychainItemInsert
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationKeychainItemModify
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationKeychainItemDelete
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
	
extern CFTypeRef kSecACLAuthorizationChangeACL 
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
extern CFTypeRef kSecACLAuthorizationChangeOwner
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);
		

/*!
	@function SecAccessGetTypeID
	@abstract Returns the type identifier of SecAccess instances.
	@result The CFTypeID of SecAccess instances.
*/
CFTypeID SecAccessGetTypeID(void);

/*!
	@function SecAccessCreate
	@abstract Creates a new SecAccessRef that is set to the currently designated system default
		configuration of a (newly created) security object. Note that the precise nature of
		this default may change between releases.
	@param descriptor The name of the item as it should appear in security dialogs
	@param trustedlist A CFArray of TrustedApplicationRefs, specifying which applications
		should be allowed to access an item without triggering confirmation dialogs.
		If NULL, defaults to (just) the application creating the item. To set no applications,
		pass a CFArray with no elements.
	@param accessRef On return, a pointer to the new access reference.
	@result A result code.  See "Security Error Codes" (SecBase.h).
*/
OSStatus SecAccessCreate(CFStringRef descriptor, CFArrayRef trustedlist, SecAccessRef *accessRef);

/*!
	@function SecAccessCreateFromOwnerAndACL
	@abstract Creates a new SecAccessRef using the owner and access control list you provide.
	@param owner A pointer to a CSSM access control list owner.
	@param aclCount An unsigned 32-bit integer representing the number of items in the access control list.
	@param acls A pointer to the access control list.
	@param On return, a pointer to the new access reference.
	@result A result code.  See "Security Error Codes" (SecBase.h).
	@discussion For 10.7 and later please use the SecAccessCreateWithOwnerAndACL API
*/
OSStatus SecAccessCreateFromOwnerAndACL(const CSSM_ACL_OWNER_PROTOTYPE *owner, uint32 aclCount, const CSSM_ACL_ENTRY_INFO *acls, SecAccessRef *accessRef)
	DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER;
	
/*!
	@function SecAccessCreateWithOwnerAndACL
	@abstract Creates a new SecAccessRef using either for a user or a group with a list of ACLs
	@param userId An user id that specifies the user to associate with this SecAccessRef.
	@param groupId A group id that specifies the group to associate with this SecAccessRef.
	@param ownerType Specifies the how the ownership of the new SecAccessRef is defined.
	@param acls A CFArrayRef of the ACLs to associate with this SecAccessRef
	@param error Optionally a pointer to a CFErrorRef to return any errors with may have occured
	@result  A pointer to the new access reference.
*/	
SecAccessRef SecAccessCreateWithOwnerAndACL(uid_t userId, gid_t groupId, SecAccessOwnerType ownerType, CFArrayRef acls, CFErrorRef *error)
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);

/*!
	@function SecAccessGetOwnerAndACL
	@abstract Retrieves the owner and the access control list of a given access.
	@param accessRef A reference to the access from which to retrieve the information.
	@param owner On return, a pointer to the access control list owner.
	@param aclCount On return, a pointer to an unsigned 32-bit integer representing the number of items in the access control list.
	@param acls On return, a pointer to the access control list.
	@result A result code.  See "Security Error Codes" (SecBase.h).
	@discussion For 10.7 and later please use the SecAccessCopyOwnerAndACL API
 */
OSStatus SecAccessGetOwnerAndACL(SecAccessRef accessRef, CSSM_ACL_OWNER_PROTOTYPE_PTR *owner, uint32 *aclCount, CSSM_ACL_ENTRY_INFO_PTR *acls)
	DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER;
	
/*!
	@function SecAccessCopyOwnerAndACL
	@abstract Retrieves the owner and the access control list of a given access.
	@param accessRef A reference to the access from which to retrieve the information.
	@param userId On return, the user id of the owner
	@param groupId On return, the group id of the owner
	@param ownerType On return, the type of owner for this AccessRef
	@param aclCount On return, a Boolean that if true specifies that the ownerId is a uid_t else it is a gid_t.
	@param aclList On return, a pointer to a new created CFArray of SecACL instances.  The caller is responsible for calling CFRelease on this array.
	@result A result code.  See "Security Error Codes" (SecBase.h).
 */	
OSStatus SecAccessCopyOwnerAndACL(SecAccessRef accessRef, uid_t* userId, gid_t* groupId, SecAccessOwnerType* ownerType, CFArrayRef* aclList)
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);

/*!
	@function SecAccessCopyACLList
	@abstract Copies all the access control lists of a given access.
	@param accessRef A reference to the access from which to retrieve the information.
	@param aclList On return, a pointer to a new created CFArray of SecACL instances.  The caller is responsible for calling CFRelease on this array.
	@result A result code.  See "Security Error Codes" (SecBase.h).
*/
OSStatus SecAccessCopyACLList(SecAccessRef accessRef, CFArrayRef *aclList);

/*!
	@function SecAccessCopySelectedACLList
	@abstract Copies selected access control lists from a given access.
	@param accessRef A reference to the access from which to retrieve the information.
	@param action An authorization tag specifying what action with which to select the action control lists.
	@param aclList On return, a pointer to the selected access control lists.
	@result A result code.  See "Security Error Codes" (SecBase.h).
	@discussion For 10.7 and later please use the SecAccessCopyMatchingACLList API
*/
OSStatus SecAccessCopySelectedACLList(SecAccessRef accessRef, CSSM_ACL_AUTHORIZATION_TAG action, CFArrayRef *aclList)
	DEPRECATED_IN_MAC_OS_X_VERSION_10_7_AND_LATER;


/*!
	@function SecAccessCopyMatchingACLList
	@abstract Copies selected access control lists from a given access.
	@param accessRef A reference to the access from which to retrieve the information.
	@param authorizationTag An authorization tag specifying what action with which to select the action control lists.
	@result A pointer to the selected access control lists.
*/
CFArrayRef SecAccessCopyMatchingACLList(SecAccessRef accessRef, CFTypeRef authorizationTag)
	__OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_NA);

#if defined(__cplusplus)
}
#endif

#endif /* !_SECURITY_SECACCESS_H_ */
