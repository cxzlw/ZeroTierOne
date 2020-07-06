/*
 * Copyright (c)2013-2020 ZeroTier, Inc.
 *
 * Use of this software is governed by the Business Source License included
 * in the LICENSE.TXT file in the project's root directory.
 *
 * Change Date: 2024-01-01
 *
 * On the date above, in accordance with the Business Source License, use
 * of this software will be governed by version 2.0 of the Apache License.
 */
/****/

/*
 * This defines the external C API for the ZeroTier network hypervisor.
 */

#ifndef ZT_ZEROTIER_API_H
#define ZT_ZEROTIER_API_H

#if defined(_WIN32) || defined(_WIN64)
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* This symbol may be defined to anything we need to put in front of API function prototypes. */
#ifndef ZT_SDK_API
#define ZT_SDK_API extern
#endif

/* ---------------------------------------------------------------------------------------------------------------- */

/**
 * Default primary UDP port for devices running a ZeroTier endpoint
 */
#define ZT_DEFAULT_PORT 9993

/**
 * IP protocol number for naked IP encapsulation (this is not currently used)
 */
#define ZT_DEFAULT_IP_PROTOCOL 193

/**
 * Ethernet type for naked Ethernet encapsulation (this is not currently used)
 */
#define ZT_DEFAULT_ETHERNET_PROTOCOL 0x9993

/**
 * Size of a standard I/O buffer as returned by getBuffer().
 */
#define ZT_BUF_SIZE 16384

/**
 * Minimum Ethernet MTU allowed on virtual (not physical) networks
 */
#define ZT_MIN_MTU 1280

/**
 * Maximum Ethernet MTU allowed on virtual (not physical) networks
 */
#define ZT_MAX_MTU 10000

/**
 * Minimum allowed physical UDP MTU (smaller values are clipped to this)
 */
#define ZT_MIN_UDP_MTU 1400

/**
 * Default UDP payload size NOT including UDP and IP overhead
 *
 * This is small enough for PPPoE and for Google Cloud's bizarrely tiny MTUs.
 * A payload size corresponding to the default 2800 byte virtual MTU fits
 * into two packets of less than or equal to this size.
 */
#define ZT_DEFAULT_UDP_MTU 1432

/**
 * Maximum physical payload size that can ever be used
 */
#define ZT_MAX_UDP_PHYSPAYLOAD 10100

/**
 * Headroom for max physical MTU
 */
#define ZT_MAX_UDP_HEADROOM 224

/**
 * Maximum payload MTU for UDP packets
 */
#define ZT_MAX_UDP_MTU (ZT_MAX_UDP_PHYSPAYLOAD + ZT_MAX_UDP_HEADROOM)

/**
 * Maximum length of network short name
 */
#define ZT_MAX_NETWORK_SHORT_NAME_LENGTH 127

/**
 * Maximum number of pushed routes on a network (via ZT in-band mechanisms)
 */
#define ZT_MAX_NETWORK_ROUTES 64

/**
 * Maximum number of statically assigned IP addresses (via ZT in-band mechanisms)
 */
#define ZT_MAX_ZT_ASSIGNED_ADDRESSES 32

/**
 * Maximum number of "specialists" on a network -- bridges, etc.
 *
 * A specialist is a node tagged with some special role like acting as
 * a promiscuous bridge, open relay, administrator, etc.
 */
#define ZT_MAX_NETWORK_SPECIALISTS 256

/**
 * Rules engine revision ID, which specifies rules engine capabilities
 */
#define ZT_RULES_ENGINE_REVISION 1

/**
 * Maximum number of base (non-capability) network rules
 */
#define ZT_MAX_NETWORK_RULES 1024

/**
 * Maximum number of capabilities per network per member
 */
#define ZT_MAX_NETWORK_CAPABILITIES 128

/**
 * Maximum number of tags per network per member
 */
#define ZT_MAX_NETWORK_TAGS 128

/**
 * Maximum number of direct network paths to a given peer
 *
 * Note that dual-stack configs may end up resulting in both IPv6 and IPv4
 * paths existing. This gives enough headroom for multipath configs with
 * dual stacks across the board.
 */
#define ZT_MAX_PEER_NETWORK_PATHS 16

/**
 * Maximum number of rules per capability object
 *
 * Capabilities normally contain only a few rules. The rules in a capability
 * should be short and to the point.
 */
#define ZT_MAX_CAPABILITY_RULES 64

/**
 * Maximum number of certificates of ownership to assign to a single network member
 *
 * Network members can have more than four IPs, etc., but right now there
 * is a protocol limit on how many COOs can be assigned. If your config needs
 * more than four authenticated IPs per node you may have personal problems.
 */
#define ZT_MAX_CERTIFICATES_OF_OWNERSHIP 4

/**
 * Packet characteristics flag: packet direction, 1 if inbound 0 if outbound
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_INBOUND 0x8000000000000000ULL

/**
 * Packet characteristics flag: multicast or broadcast destination MAC
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_MULTICAST 0x4000000000000000ULL

/**
 * Packet characteristics flag: broadcast destination MAC
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_BROADCAST 0x2000000000000000ULL

/**
 * Packet characteristics flag: sending IP address has a certificate of ownership
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_SENDER_IP_AUTHENTICATED 0x1000000000000000ULL

/**
 * Packet characteristics flag: sending MAC address has a certificate of ownership
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_SENDER_MAC_AUTHENTICATED 0x0800000000000000ULL

/**
 * Packet characteristics flag: TCP left-most reserved bit
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_TCP_RESERVED_0 0x0000000000000800ULL

/**
 * Packet characteristics flag: TCP middle reserved bit
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_TCP_RESERVED_1 0x0000000000000400ULL

/**
 * Packet characteristics flag: TCP right-most reserved bit
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_TCP_RESERVED_2 0x0000000000000200ULL

/**
 * Packet characteristics flag: TCP NS flag
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_TCP_NS 0x0000000000000100ULL

/**
 * Packet characteristics flag: TCP CWR flag
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_TCP_CWR 0x0000000000000080ULL

/**
 * Packet characteristics flag: TCP ECE flag
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_TCP_ECE 0x0000000000000040ULL

/**
 * Packet characteristics flag: TCP URG flag
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_TCP_URG 0x0000000000000020ULL

/**
 * Packet characteristics flag: TCP ACK flag
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_TCP_ACK 0x0000000000000010ULL

/**
 * Packet characteristics flag: TCP PSH flag
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_TCP_PSH 0x0000000000000008ULL

/**
 * Packet characteristics flag: TCP RST flag
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_TCP_RST 0x0000000000000004ULL

/**
 * Packet characteristics flag: TCP SYN flag
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_TCP_SYN 0x0000000000000002ULL

/**
 * Packet characteristics flag: TCP FIN flag
 */
#define ZT_RULE_PACKET_CHARACTERISTICS_TCP_FIN 0x0000000000000001ULL

/* ---------------------------------------------------------------------------------------------------------------- */

/**
 * Identity type codes (must be the same as Identity.hpp).
 */
enum ZT_IdentityType
{
	ZT_IDENTITY_TYPE_C25519 = 0, /* C25519/Ed25519 */
	ZT_IDENTITY_TYPE_P384 =   1  /* Combined C25519/NIST-P-384 key */
};

/**
 * ZeroTier identity (address plus keys)
 */
typedef void ZT_Identity;

/**
 * Locator is a signed list of endpoints
 */
typedef void ZT_Locator;

/**
 * Full identity fingerprint with address and 384-bit hash of public key(s)
 */
typedef struct
{
	/**
	 * Short address (only least significant 40 bits are used)
	 */
	uint64_t address;

	/**
	 * 384-bit hash of identity public key(s)
	 */
	uint8_t hash[48];
} ZT_Fingerprint;

/**
 * Maximum length of string fields in certificates
 */
#define ZT_CERTIFICATE_MAX_STRING_LENGTH 127

/**
 * Certificate is a root CA
 */
#define ZT_CERTIFICATE_LOCAL_TRUST_FLAG_ROOT_CA 0x0001U

/**
 * Certificate's subject describes a set of roots
 */
#define ZT_CERTIFICATE_LOCAL_TRUST_FLAG_ZEROTIER_ROOT_SET 0x0002U

/**
 * Size of a unique ID of the given key type (with type prefix byte)
 */
#define ZT_CERTIFICATE_UNIQUE_ID_TYPE_NIST_P_384_SIZE 50

/**
 * Size of the private key corresponding to a unique ID of the given type.
 */
#define ZT_CERTIFICATE_UNIQUE_ID_TYPE_NIST_P_384_PRIVATE_SIZE 48

/**
 * Unique ID types supported for certificate subject unique IDs
 */
enum ZT_CertificateUniqueIdType
{
	/**
	 * Public key type for NIST P-384 public keys used as subject unique IDs.
	 */
	ZT_CERTIFICATE_UNIQUE_ID_TYPE_NIST_P_384 = 1
};

/**
 * Errors returned by functions that verify or handle certificates.
 */
enum ZT_CertificateError
{
	/**
	 * No error (certificate is valid or operation was successful)
	 */
	ZT_CERTIFICATE_ERROR_NONE = 0,

	/**
	 * A newer certificate with the same issuer and subject serial plus CN exists.
	 */
	ZT_CERTIFICATE_ERROR_HAVE_NEWER_CERT = 1,

	/**
	 * Certificate format is invalid or required fields are missing
	 */
	ZT_CERTIFICATE_ERROR_INVALID_FORMAT = -1,

	/**
	 * One or more identities in the certificate are invalid or fail consistency check
	 */
	ZT_CERTIFICATE_ERROR_INVALID_IDENTITY = -2,

	/**
	 * Certificate primary signature is invalid
	 */
	ZT_CERTIFICATE_ERROR_INVALID_PRIMARY_SIGNATURE = -3,

	/**
	 * Full chain validation of certificate failed
	 */
	ZT_CERTIFICATE_ERROR_INVALID_CHAIN = -4,

	/**
	 * One or more signed components (e.g. a Locator) has an invalid signature.
	 */
	ZT_CERTIFICATE_ERROR_INVALID_COMPONENT_SIGNATURE = -5,

	/**
	 * Unique ID proof signature in subject was not valid.
	 */
	ZT_CERTIFICATE_ERROR_INVALID_UNIQUE_ID_PROOF = -6,

	/**
	 * Certificate is missing a required field
	 */
	ZT_CERTIFICATE_ERROR_MISSING_REQUIRED_FIELDS = -7,

	/**
	 * Certificate is expired or not yet in effect
	 */
	ZT_CERTIFICATE_ERROR_OUT_OF_VALID_TIME_WINDOW = -8
};

/**
 * Information about a real world entity.
 *
 * These fields are all optional and are all taken from the
 * most common fields present in X509 certificates.
 */
typedef struct
{
	char serialNo[ZT_CERTIFICATE_MAX_STRING_LENGTH + 1];
	char commonName[ZT_CERTIFICATE_MAX_STRING_LENGTH + 1];
	char country[ZT_CERTIFICATE_MAX_STRING_LENGTH + 1];
	char organization[ZT_CERTIFICATE_MAX_STRING_LENGTH + 1];
	char unit[ZT_CERTIFICATE_MAX_STRING_LENGTH + 1];
	char locality[ZT_CERTIFICATE_MAX_STRING_LENGTH + 1];
	char province[ZT_CERTIFICATE_MAX_STRING_LENGTH + 1];
	char streetAddress[ZT_CERTIFICATE_MAX_STRING_LENGTH + 1];
	char postalCode[ZT_CERTIFICATE_MAX_STRING_LENGTH + 1];
	char email[ZT_CERTIFICATE_MAX_STRING_LENGTH + 1];
	char url[ZT_CERTIFICATE_MAX_STRING_LENGTH + 1];
	char host[ZT_CERTIFICATE_MAX_STRING_LENGTH + 1];
} ZT_Certificate_Name;

/**
 * Identity and optional locator to help find a node on physical networks.
 */
typedef struct
{
	/**
	 * Identity (never NULL)
	 */
	const ZT_Identity *identity;

	/**
	 * Locator, or NULL if none
	 */
	const ZT_Locator *locator;
} ZT_Certificate_Identity;

/**
 * ID and primary controller for a network
 */
typedef struct
{
	/**
	 * Network ID
	 */
	uint64_t id;

	/**
	 * Full fingerprint of primary controller
	 */
	ZT_Fingerprint controller;
} ZT_Certificate_Network;

/**
 * Identification certificate subject
 */
typedef struct
{
	/**
	 * Timestamp of subject, can also be a revision ID for this subject's name.
	 */
	int64_t timestamp;

	/**
	 * Identities and optional locators of nodes
	 */
	ZT_Certificate_Identity *identities;

	/**
	 * Networks owned by this entity
	 */
	ZT_Certificate_Network *networks;

	/**
	 * Serial numbers of other certificates being signed (each is 48 bytes / 384 bits)
	 */
	const uint8_t *const *certificates;

	/**
	 * URLs that can be consulted for updates to this certificate.
	 */
	const char *const *updateURLs;

	/**
	 * Number of identities
	 */
	unsigned int identityCount;

	/**
	 * Number of networks
	 */
	unsigned int networkCount;

	/**
	 * Number of certificates
	 */
	unsigned int certificateCount;

	/**
	 * Number of update URLs
	 */
	unsigned int updateURLCount;

	/**
	 * Information about owner of items.
	 */
	ZT_Certificate_Name name;

	/**
	 * Globally unique ID for this subject
	 *
	 * Unique IDs are actually public keys. Their size makes them globally
	 * unique (if generated from good randomness) to within ridiculous
	 * probability bounds. If a subject has a unique ID it must also have
	 * a unique ID proof signature, which is the signature of the subject
	 * with the private key corresponding to its unique ID.
	 *
	 * This allows subjects to "own" themselves and exist independent of
	 * CAs or delegated signers. It also allows a certificate for a given
	 * subject to be updated.
	 *
	 * Subject unique IDs are optional. If no unique ID is specified these
	 * and their corresponding size fields must be empty/zero.
	 *
	 * A subject is valid if it has no unique ID or has one with a valid
	 * proof signature.
	 */
	const uint8_t *uniqueId;

	/**
	 * Signature proving ownership of unique ID.
	 */
	const uint8_t *uniqueIdProofSignature;

	/**
	 * Size of unique ID in bytes or 0 if none.
	 */
	unsigned int uniqueIdSize;

	/**
	 * Proof signature size or 0 if none.
	 */
	unsigned int uniqueIdProofSignatureSize;
} ZT_Certificate_Subject;

/**
 * Certificate
 *
 * This is designed to be compatible with x509 certificate interfaces,
 * presenting similar concepts and fields.
 *
 * It's not X509 because we want to keep ZeroTier clean, as simple as
 * possible, small, and secure. X509 is both bloated and a security
 * disaster as it's very hard to implement correctly.
 */
typedef struct
{
	/**
	 * Serial number, a SHA384 hash of this certificate.
	 */
	uint8_t serialNo[48];

	/**
	 * Flags indicating certificate usage and any other attributes.
	 */
	uint64_t flags;

	/**
	 * Certificate timestamp in milliseconds since epoch.
	 */
	int64_t timestamp;

	/**
	 * Valid time range: not before, not after.
	 */
	int64_t validity[2];

	/**
	 * Subject of certificate
	 */
	ZT_Certificate_Subject subject;

	/**
	 * Issuer node identity and public key(s).
	 */
	const ZT_Identity *issuer;

	/**
	 * Issuer information
	 */
	ZT_Certificate_Name issuerName;

	/**
	 * Extended attributes set by issuer (in Dictionary format, NULL if none)
	 */
	const uint8_t *extendedAttributes;

	/**
	 * Size of extended attributes field in bytes
	 */
	unsigned int extendedAttributesSize;

	/**
	 * Maximum path length from this certificate toward further certificates.
	 *
	 * Subjects may sign other certificates whose path lengths are less than
	 * this value. A value of zero indicates that no identification certificates
	 * may be signed (not a CA).
	 */
	unsigned int maxPathLength;

	/**
	 * Signature by issuer (algorithm determined by identity type).
	 */
	const uint8_t *signature;

	/**
	 * Size of signature in bytes.
	 */
	unsigned int signatureSize;
} ZT_Certificate;

/**
 * Credential type IDs
 */
enum ZT_CredentialType
{
	ZT_CREDENTIAL_TYPE_NULL =        0,
	ZT_CREDENTIAL_TYPE_COM =         1,
	ZT_CREDENTIAL_TYPE_CAPABILITY =  2,
	ZT_CREDENTIAL_TYPE_TAG =         3,
	ZT_CREDENTIAL_TYPE_COO =         4,
	ZT_CREDENTIAL_TYPE_REVOCATION =  6
};

/**
 * Endpoint address and protocol types
 *
 * Most of these are not currently implemented and are just reserved
 * for future use.
 */
enum ZT_EndpointType
{
	ZT_ENDPOINT_TYPE_NIL =           0,  /* Nil/empty endpoint */
	ZT_ENDPOINT_TYPE_ZEROTIER =      1,  /* ZeroTier relaying (address+fingerprint) */
	ZT_ENDPOINT_TYPE_ETHERNET =      2,  /* Ethernet with ethertype 0x9993 */
	ZT_ENDPOINT_TYPE_WIFI_DIRECT =   3,  /* Ethernet using WiFi direct */
	ZT_ENDPOINT_TYPE_BLUETOOTH =     4,  /* Bluetooth (same address type as Ethernet) */
	ZT_ENDPOINT_TYPE_IP =            5,  /* Naked IP (protocol 193) */
	ZT_ENDPOINT_TYPE_IP_UDP =        6,  /* IP/UDP */
	ZT_ENDPOINT_TYPE_IP_TCP =        7,  /* IP/TCP */
	ZT_ENDPOINT_TYPE_IP_HTTP =       8   /* IP/HTTP encapsulation */
};

/**
 * Flag indicating that VL1 tracing should be generated
 */
#define ZT_TRACE_FLAG_VL1           0x01

/**
 * Flag indicating that VL2 (virtual network) tracing should be generated
 */
#define ZT_TRACE_FLAG_VL2           0x02

/**
 * Flag indicating that VL2 network filter tracing should be generated (separate because this can be very verbose)
 */
#define ZT_TRACE_FLAG_VL2_FILTER    0x04

/**
 * Flag indicating that VL2 multicast propagation should be reported
 */
#define ZT_TRACE_FLAG_VL2_MULTICAST 0x08

/**
 * Trace event types
 *
 * All trace event structures start with a size and type.
 */
enum ZT_TraceEventType
{
	ZT_TRACE_UNEXPECTED_ERROR =             0,
	ZT_TRACE_VL1_RESETTING_PATHS_IN_SCOPE = 1,
	ZT_TRACE_VL1_TRYING_NEW_PATH =          2,
	ZT_TRACE_VL1_LEARNED_NEW_PATH =         3,
	ZT_TRACE_VL1_INCOMING_PACKET_DROPPED =  4,
	ZT_TRACE_VL2_OUTGOING_FRAME_DROPPED =   100,
	ZT_TRACE_VL2_INCOMING_FRAME_DROPPED =   101,
	ZT_TRACE_VL2_NETWORK_CONFIG_REQUESTED = 102,
	ZT_TRACE_VL2_NETWORK_FILTER =           103
};

/**
 * Trace VL1 packet drop reasons
 */
enum ZT_TracePacketDropReason
{
	ZT_TRACE_PACKET_DROP_REASON_UNSPECIFIED =             0,
	ZT_TRACE_PACKET_DROP_REASON_PEER_TOO_OLD =            1,
	ZT_TRACE_PACKET_DROP_REASON_MALFORMED_PACKET =        2,
	ZT_TRACE_PACKET_DROP_REASON_MAC_FAILED =              3,
	ZT_TRACE_PACKET_DROP_REASON_RATE_LIMIT_EXCEEDED =     4,
	ZT_TRACE_PACKET_DROP_REASON_INVALID_OBJECT =          5,
	ZT_TRACE_PACKET_DROP_REASON_INVALID_COMPRESSED_DATA = 6,
	ZT_TRACE_PACKET_DROP_REASON_UNRECOGNIZED_VERB =       7,
	ZT_TRACE_PACKET_DROP_REASON_REPLY_NOT_EXPECTED =      8
};

/**
 * Trace VL2 frame drop reasons
 */
enum ZT_TraceFrameDropReason
{
	ZT_TRACE_FRAME_DROP_REASON_UNSPECIFIED =                          0,
	ZT_TRACE_FRAME_DROP_REASON_BRIDGING_NOT_ALLOWED_REMOTE =          1,
	ZT_TRACE_FRAME_DROP_REASON_BRIDGING_NOT_ALLOWED_LOCAL =           2,
	ZT_TRACE_FRAME_DROP_REASON_MULTICAST_DISABLED =                   3,
	ZT_TRACE_FRAME_DROP_REASON_BROADCAST_DISABLED =                   4,
	ZT_TRACE_FRAME_DROP_REASON_FILTER_BLOCKED =                       5,
	ZT_TRACE_FRAME_DROP_REASON_FILTER_BLOCKED_AT_BRIDGE_REPLICATION = 6,
	ZT_TRACE_FRAME_DROP_REASON_PERMISSION_DENIED =                    7
};

/**
 * Reasons for credential rejection
 */
enum ZT_TraceCredentialRejectionReason
{
	ZT_TRACE_CREDENTIAL_REJECTION_REASON_SIGNATURE_VERIFICATION_FAILED = 1,
	ZT_TRACE_CREDENTIAL_REJECTION_REASON_REVOKED =                       2,
	ZT_TRACE_CREDENTIAL_REJECTION_REASON_OLDER_THAN_LATEST =             3,
	ZT_TRACE_CREDENTIAL_REJECTION_REASON_INVALID =                       4
};

#define ZT_TRACE_FIELD_TYPE                               "t"
#define ZT_TRACE_FIELD_CODE_LOCATION                      "c"
#define ZT_TRACE_FIELD_ENDPOINT                           "e"
#define ZT_TRACE_FIELD_OLD_ENDPOINT                       "oe"
#define ZT_TRACE_FIELD_NEW_ENDPOINT                       "ne"
#define ZT_TRACE_FIELD_TRIGGER_FROM_ENDPOINT              "te"
#define ZT_TRACE_FIELD_TRIGGER_FROM_PACKET_ID             "ti"
#define ZT_TRACE_FIELD_TRIGGER_FROM_PACKET_VERB           "tv"
#define ZT_TRACE_FIELD_TRIGGER_FROM_PEER_FINGERPRINT_HASH "tp"
#define ZT_TRACE_FIELD_MESSAGE                            "m"
#define ZT_TRACE_FIELD_RESET_ADDRESS_SCOPE                "rs"
#define ZT_TRACE_FIELD_IDENTITY_FINGERPRINT_HASH          "f"
#define ZT_TRACE_FIELD_PACKET_ID                          "p"
#define ZT_TRACE_FIELD_PACKET_VERB                        "v"
#define ZT_TRACE_FIELD_PACKET_HOPS                        "h"
#define ZT_TRACE_FIELD_NETWORK_ID                         "n"
#define ZT_TRACE_FIELD_REASON                             "r"
#define ZT_TRACE_FIELD_SOURCE_MAC                         "sm"
#define ZT_TRACE_FIELD_DEST_MAC                           "dm"
#define ZT_TRACE_FIELD_ETHERTYPE                          "et"
#define ZT_TRACE_FIELD_VLAN_ID                            "vlid"
#define ZT_TRACE_FIELD_FRAME_LENGTH                       "fl"
#define ZT_TRACE_FIELD_FRAME_DATA                         "fd"
#define ZT_TRACE_FIELD_FLAG_CREDENTIAL_REQUEST_SENT       "crs"
#define ZT_TRACE_FIELD_PRIMARY_RULE_SET_LOG               "rL"
#define ZT_TRACE_FIELD_MATCHING_CAPABILITY_RULE_SET_LOG   "caRL"
#define ZT_TRACE_FIELD_MATCHING_CAPABILITY_ID             "caID"
#define ZT_TRACE_FIELD_MATCHING_CAPABILITY_TIMESTAMP      "caTS"
#define ZT_TRACE_FIELD_SOURCE_ZT_ADDRESS                  "sz"
#define ZT_TRACE_FIELD_DEST_ZT_ADDRESS                    "dz"
#define ZT_TRACE_FIELD_RULE_FLAG_NOTEE                    "rNT"
#define ZT_TRACE_FIELD_RULE_FLAG_INBOUND                  "rIN"
#define ZT_TRACE_FIELD_RULE_FLAG_ACCEPT                   "rACC"
#define ZT_TRACE_FIELD_CREDENTIAL_ID                      "crID"
#define ZT_TRACE_FIELD_CREDENTIAL_TYPE                    "crT"
#define ZT_TRACE_FIELD_CREDENTIAL_TIMESTAMP               "crTS"

/**
 * Function return code: OK (0) or error results
 *
 * Use ZT_ResultCode_isFatal() to check for a fatal error. If a fatal error
 * occurs, the node should be considered to not be working correctly. These
 * indicate serious problems like an inaccessible data store or a compile
 * problem.
 */
enum ZT_ResultCode
{
	/**
	 * Operation completed normally
	 */
	ZT_RESULT_OK = 0,

	/* Fatal errors (>100, <1000) */

	/**
	 * Ran out of memory
	 */
	ZT_RESULT_FATAL_ERROR_OUT_OF_MEMORY = 100,

	/**
	 * Data store is not writable or has failed
	 */
	ZT_RESULT_FATAL_ERROR_DATA_STORE_FAILED = 101,

	/**
	 * Internal error fatal to the instance
	 */
	ZT_RESULT_FATAL_ERROR_INTERNAL = 102,

	/* Non-fatal errors (>1000) */

	/**
	 * Network ID not valid
	 */
	ZT_RESULT_ERROR_NETWORK_NOT_FOUND = 1000,

	/**
	 * The requested operation is not supported on this version or build
	 */
	ZT_RESULT_ERROR_UNSUPPORTED_OPERATION = 1001,

	/**
	 * The requested operation was given a bad parameter or was called in an invalid state
	 */
	ZT_RESULT_ERROR_BAD_PARAMETER = 1002,

	/**
	 * A credential or other object was supplied that failed cryptographic signature or integrity check
	 */
	ZT_RESULT_ERROR_INVALID_CREDENTIAL = 1003,

	/**
	 * An object collides with another object in some way (meaning is object-specific)
	 */
	ZT_RESULT_ERROR_COLLIDING_OBJECT = 1004,

	/**
	 * An internal error occurred, but one that is not fatal to the whole instance
	 */
	ZT_RESULT_ERROR_INTERNAL = 1005
};

/**
 * Macro to check for a fatal error result code
 *
 * @param x Result code
 * @return True if result code indicates a fatal error
 */
#define ZT_ResultCode_isFatal(x) ((((int)(x)) >= 100)&&(((int)(x)) < 1000))

/**
 * Status codes sent to status update callback when things happen
 */
enum ZT_Event
{
	/**
	 * Node has been initialized
	 *
	 * This is the first event generated, and is always sent. It may occur
	 * before Node's constructor returns.
	 *
	 * Meta-data: none
	 */
	ZT_EVENT_UP = 0,

	/**
	 * Node appears offline
	 *
	 * This indicates that the node doesn't seem to be able to reach anything,
	 * or hasn't for a while. It's not a hard instantaneous thing.
	 *
	 * Meta-data: none
	 */
	ZT_EVENT_OFFLINE = 1,

	/**
	 * Node appears online
	 *
	 * This indicates that the node was offline but now seems to be able to
	 * reach something. Like OFFLINE it's not a hard instantaneous thing but
	 * more of an indicator for UI reporting purposes.
	 *
	 * Meta-data: none
	 */
	ZT_EVENT_ONLINE = 2,

	/**
	 * Node is shutting down
	 *
	 * This is generated within Node's destructor when it is being shut down.
	 * It's done for convenience in case you want to clean up anything during
	 * node shutdown in your node event handler.
	 *
	 * Meta-data: none
	 */
	ZT_EVENT_DOWN = 3,

	/* 4 once signaled identity collision but this is no longer an error */

	/**
	 * Trace (debugging) message
	 *
	 * These events are only generated if this is a TRACE-enabled build.
	 * This is for local debug traces, not remote trace diagnostics.
	 *
	 * Meta-data: struct of type ZT_Trace_*
	 */
	ZT_EVENT_TRACE = 5,

	/**
	 * VERB_USER_MESSAGE received
	 *
	 * These are generated when a VERB_USER_MESSAGE packet is received via
	 * ZeroTier VL1. This can be used for below-VL2 in-band application
	 * specific signaling over the ZeroTier protocol.
	 *
	 * It's up to you to ensure that you handle these in a way that does
	 * not introduce a remote security vulnerability into your app! If
	 * your USER_MESSAGE code has a buffer overflow or other vulnerability
	 * then your app will be vulnerable and this is not ZT's fault. :)
	 *
	 * Meta-data: ZT_UserMessage structure
	 */
	ZT_EVENT_USER_MESSAGE = 6
};

/**
 * User message used with ZT_EVENT_USER_MESSAGE
 *
 * These are direct VL1 P2P messages for application use. Encryption and
 * authentication in the ZeroTier protocol will guarantee the origin
 * address and message content, but you are responsible for any other
 * levels of authentication or access control that are required. Any node
 * in the world can send you a user message! (Unless your network is air
 * gapped.)
 *
 * Pointers to id and data might not remain valid after the event is
 * received.
 */
typedef struct
{
	/**
	 * Identity of sender
	 */
	const ZT_Identity *id;

	/**
	 * User message type ID
	 */
	uint64_t typeId;

	/**
	 * User message data
	 */
	const void *data;

	/**
	 * Length of data in bytes
	 */
	unsigned int length;
} ZT_UserMessage;

/**
 * Current node status
 */
typedef struct
{
	/**
	 * 40-bit ZeroTier address of this node
	 */
	uint64_t address;

	/**
	 * Actual identity object for this node
	 */
	const ZT_Identity *identity;

	/**
	 * Public identity in string-serialized form (safe to send to others)
	 *
	 * This pointer will remain valid as long as the node exists.
	 */
	const char *publicIdentity;

	/**
	 * Full identity including secret key in string-serialized form
	 *
	 * This pointer will remain valid as long as the node exists.
	 */
	const char *secretIdentity;

	/**
	 * True if some kind of connectivity appears available
	 */
	int online;
} ZT_NodeStatus;

/**
 * Virtual network status codes
 */
enum ZT_VirtualNetworkStatus
{
	/**
	 * Waiting for network configuration (also means revision == 0)
	 */
	ZT_NETWORK_STATUS_REQUESTING_CONFIGURATION = 0,

	/**
	 * Configuration received and we are authorized
	 */
	ZT_NETWORK_STATUS_OK = 1,

	/**
	 * Netconf master told us 'nope'
	 */
	ZT_NETWORK_STATUS_ACCESS_DENIED = 2,

	/**
	 * Netconf master exists, but this virtual network does not
	 */
	ZT_NETWORK_STATUS_NOT_FOUND = 3
};

/**
 * Virtual network type codes
 */
enum ZT_VirtualNetworkType
{
	/**
	 * Private networks are authorized via membership credentials
	 */
	ZT_NETWORK_TYPE_PRIVATE = 0,

	/**
	 * Public networks have no access control -- they'll always be AUTHORIZED
	 */
	ZT_NETWORK_TYPE_PUBLIC = 1
};

/**
 * The type of a virtual network rules table entry
 *
 * These must be from 0 to 63 since the most significant two bits of each
 * rule type are NOT (MSB) and AND/OR.
 *
 * Each rule is composed of zero or more MATCHes followed by an ACTION.
 * An ACTION with no MATCHes is always taken.
 */
enum ZT_VirtualNetworkRuleType
{
	/* 0 to 15 reserved for actions */

	/**
	 * Drop frame
	 */
	ZT_NETWORK_RULE_ACTION_DROP = 0,

	/**
	 * Accept and pass frame
	 */
	ZT_NETWORK_RULE_ACTION_ACCEPT = 1,

	/**
	 * Forward a copy of this frame to an observer (by ZT address)
	 */
	ZT_NETWORK_RULE_ACTION_TEE = 2,

	/**
	 * Exactly like TEE but mandates ACKs from observer
	 */
	ZT_NETWORK_RULE_ACTION_WATCH = 3,

	/**
	 * Drop and redirect this frame to another node (by ZT address)
	 */
	ZT_NETWORK_RULE_ACTION_REDIRECT = 4,

	/**
	 * Stop evaluating rule set (drops unless there are capabilities, etc.)
	 */
	ZT_NETWORK_RULE_ACTION_BREAK = 5,

	/**
	 * Place a matching frame in the specified QoS bucket
	 */
	ZT_NETWORK_RULE_ACTION_PRIORITY = 6,

	/**
	 * Maximum ID for an ACTION, anything higher is a MATCH
	 */
	ZT_NETWORK_RULE_ACTION__MAX_ID = 15,

	// 16 to 63 reserved for match criteria

	ZT_NETWORK_RULE_MATCH_SOURCE_ZEROTIER_ADDRESS = 24,
	ZT_NETWORK_RULE_MATCH_DEST_ZEROTIER_ADDRESS = 25,
	ZT_NETWORK_RULE_MATCH_VLAN_ID = 26,
	ZT_NETWORK_RULE_MATCH_VLAN_PCP = 27,
	ZT_NETWORK_RULE_MATCH_VLAN_DEI = 28,
	ZT_NETWORK_RULE_MATCH_MAC_SOURCE = 29,
	ZT_NETWORK_RULE_MATCH_MAC_DEST = 30,
	ZT_NETWORK_RULE_MATCH_IPV4_SOURCE = 31,
	ZT_NETWORK_RULE_MATCH_IPV4_DEST = 32,
	ZT_NETWORK_RULE_MATCH_IPV6_SOURCE = 33,
	ZT_NETWORK_RULE_MATCH_IPV6_DEST = 34,
	ZT_NETWORK_RULE_MATCH_IP_TOS = 35,
	ZT_NETWORK_RULE_MATCH_IP_PROTOCOL = 36,
	ZT_NETWORK_RULE_MATCH_ETHERTYPE = 37,
	ZT_NETWORK_RULE_MATCH_ICMP = 38,
	ZT_NETWORK_RULE_MATCH_IP_SOURCE_PORT_RANGE = 39,
	ZT_NETWORK_RULE_MATCH_IP_DEST_PORT_RANGE = 40,
	ZT_NETWORK_RULE_MATCH_CHARACTERISTICS = 41,
	ZT_NETWORK_RULE_MATCH_FRAME_SIZE_RANGE = 42,
	ZT_NETWORK_RULE_MATCH_RANDOM = 43,
	ZT_NETWORK_RULE_MATCH_TAGS_DIFFERENCE = 44,
	ZT_NETWORK_RULE_MATCH_TAGS_BITWISE_AND = 45,
	ZT_NETWORK_RULE_MATCH_TAGS_BITWISE_OR = 46,
	ZT_NETWORK_RULE_MATCH_TAGS_BITWISE_XOR = 47,
	ZT_NETWORK_RULE_MATCH_TAGS_EQUAL = 48,
	ZT_NETWORK_RULE_MATCH_TAG_SENDER = 49,
	ZT_NETWORK_RULE_MATCH_TAG_RECEIVER = 50,
	ZT_NETWORK_RULE_MATCH_INTEGER_RANGE = 51,

	/**
	 * Maximum ID allowed for a MATCH entry in the rules table
	 */
	ZT_NETWORK_RULE_MATCH__MAX_ID = 63
};

/**
 * Network flow rule
 *
 * Rules are stored in a table in which one or more match entries is followed
 * by an action. If more than one match precedes an action, the rule is
 * the AND of all matches. An action with no match is always taken since it
 * matches anything. If nothing matches, the default action is DROP.
 *
 * This is designed to be a more memory-efficient way of storing rules than
 * a wide table, yet still fast and simple to access in code.
 */
typedef struct
{
	/**
	 * Type and flags
	 *
	 * Bits are: NOTTTTTT
	 *
	 * N - If true, sense of match is inverted (no effect on actions)
	 * O - If true, result is ORed with previous instead of ANDed (no effect on actions)
	 * T - Rule or action type
	 *
	 * AND with 0x3f to get type, 0x80 to get NOT bit, and 0x40 to get OR bit.
	 */
	uint8_t t;

	/**
	 * Union containing the value of this rule -- which field is used depends on 't'
	 */
	union {
		/**
		 * IPv6 address in big-endian / network byte order and netmask bits
		 */
		struct {
			uint8_t ip[16];
			uint8_t mask;
		} ipv6;

		/**
		 * IPv4 address in big-endian / network byte order
		 */
		struct {
			uint32_t ip;
			uint8_t mask;
		} ipv4;

		/**
		 * Integer range match in packet payload
		 *
		 * This allows matching of ranges of integers up to 64 bits wide where
		 * the range is +/- INT32_MAX. It's packed this way so it fits in 16
		 * bytes and doesn't enlarge the overall size of this union.
		 */
		struct {
			uint64_t start; /* integer range start */
			uint32_t end;   /* end of integer range (relative to start, inclusive, 0 for equality w/start) */
			uint16_t idx;   /* index in packet of integer */
			uint8_t format; /* bits in integer (range 1-64, ((format&63)+1)) and endianness (MSB 1 for little, 0 for big) */
		} intRange;

		/**
		 * Packet characteristic flags being matched
		 */
		uint64_t characteristics;

		/**
		 * IP port range -- start-end inclusive -- host byte order
		 */
		uint16_t port[2];

		/**
		 * 40-bit ZeroTier address (in least significant bits, host byte order)
		 */
		uint64_t zt;

		/**
		 * 0 = never, UINT32_MAX = always
		 */
		uint32_t randomProbability;

		/**
		 * 48-bit Ethernet MAC address in big-endian order
		 */
		uint8_t mac[6];

		/**
		 * VLAN ID in host byte order
		 */
		uint16_t vlanId;

		/**
		 * VLAN PCP (least significant 3 bits)
		 */
		uint8_t vlanPcp;

		/**
		 * VLAN DEI (single bit / boolean)
		 */
		uint8_t vlanDei;

		/**
		 * Ethernet type in host byte order
		 */
		uint16_t etherType;

		/**
		 * IP protocol
		 */
		uint8_t ipProtocol;

		/**
		 * IP type of service a.k.a. DSCP field
		 */
		struct {
			uint8_t mask;
			uint8_t value[2];
		} ipTos;

		/**
		 * Ethernet packet size in host byte order (start-end, inclusive)
		 */
		uint16_t frameSize[2];

		/**
		 * ICMP type and code
		 */
		struct {
			uint8_t type;  /* ICMP type, always matched */
			uint8_t code;  /* ICMP code if matched */
			uint8_t flags; /* flag 0x01 means also match code, otherwise only match type */
		} icmp;

		/**
		 * For tag-related rules
		 */
		struct {
			uint32_t id;
			uint32_t value;
		} tag;

		/**
		 * Destinations for TEE and REDIRECT
		 */
		struct {
			uint64_t address;
			uint32_t flags;
			uint16_t length;
		} fwd;

		/**
		 * Quality of Service (QoS) bucket we want a frame to be placed in
		 */
		uint8_t qosBucket;
	} v;
} ZT_VirtualNetworkRule;

/**
 * A route to be pushed on a virtual network
 */
typedef struct
{
	/**
	 * Target network / netmask bits (in port field) or NULL or 0.0.0.0/0 for default
	 */
	struct sockaddr_storage target;

	/**
	 * Gateway IP address (port ignored) or NULL (family == 0) for LAN-local (no gateway)
	 */
	struct sockaddr_storage via;

	/**
	 * Route flags
	 */
	uint16_t flags;

	/**
	 * Route metric
	 */
	uint16_t metric;
} ZT_VirtualNetworkRoute;

/**
 * An Ethernet multicast group
 */
typedef struct
{
	/**
	 * MAC address (least significant 48 bits)
	 */
	uint64_t mac;

	/**
	 * Additional distinguishing information (usually zero)
	 */
	unsigned long adi;
} ZT_MulticastGroup;

/**
 * Virtual network configuration update type
 */
enum ZT_VirtualNetworkConfigOperation
{
	/**
	 * Network is coming up (either for the first time or after service restart)
	 */
	ZT_VIRTUAL_NETWORK_CONFIG_OPERATION_UP = 1,

	/**
	 * Network configuration has been updated
	 */
	ZT_VIRTUAL_NETWORK_CONFIG_OPERATION_CONFIG_UPDATE = 2,

	/**
	 * Network is going down (not permanently)
	 */
	ZT_VIRTUAL_NETWORK_CONFIG_OPERATION_DOWN = 3,

	/**
	 * Network is going down permanently (leave/delete)
	 */
	ZT_VIRTUAL_NETWORK_CONFIG_OPERATION_DESTROY = 4
};

/**
 * Virtual network configuration
 */
typedef struct
{
	/**
	 * 64-bit ZeroTier network ID
	 */
	uint64_t nwid;

	/**
	 * Ethernet MAC (48 bits) that should be assigned to port
	 */
	uint64_t mac;

	/**
	 * Network name (from network configuration master)
	 */
	char name[ZT_MAX_NETWORK_SHORT_NAME_LENGTH + 1];

	/**
	 * Network configuration request status
	 */
	enum ZT_VirtualNetworkStatus status;

	/**
	 * Network type
	 */
	enum ZT_VirtualNetworkType type;

	/**
	 * Maximum interface MTU
	 */
	unsigned int mtu;

	/**
	 * If nonzero, this port is allowed to bridge to other networks
	 *
	 * This is informational. If this is false (0), bridged packets will simply
	 * be dropped and bridging won't work.
	 */
	int bridge;

	/**
	 * If nonzero, this network supports and allows broadcast (ff:ff:ff:ff:ff:ff) traffic
	 */
	int broadcastEnabled;

	/**
	 * Revision number as reported by controller or 0 if still waiting for config
	 */
	unsigned long netconfRevision;

	/**
	 * Number of assigned addresses
	 */
	unsigned int assignedAddressCount;

	/**
	 * ZeroTier-assigned addresses (in sockaddr_storage structures)
	 *
	 * For IP, the port number of the sockaddr_XX structure contains the number
	 * of bits in the address netmask. Only the IP address and port are used.
	 * Other fields like interface number can be ignored.
	 *
	 * This is only used for ZeroTier-managed address assignments sent by the
	 * virtual network's configuration master.
	 */
	struct sockaddr_storage assignedAddresses[ZT_MAX_ZT_ASSIGNED_ADDRESSES];

	/**
	 * Number of ZT-pushed routes
	 */
	unsigned int routeCount;

	/**
	 * Routes (excluding those implied by assigned addresses and their masks)
	 */
	ZT_VirtualNetworkRoute routes[ZT_MAX_NETWORK_ROUTES];
} ZT_VirtualNetworkConfig;

/**
 * A list of networks
 */
typedef struct
{
	ZT_VirtualNetworkConfig *networks;
	unsigned long networkCount;
} ZT_VirtualNetworkList;

/**
 * Address where this node could be reached via an external interface
 */
typedef struct
{
	/**
	 * IP and port as would be reachable by external nodes
	 */
	struct sockaddr_storage address;

	/**
	 * If nonzero this address is static and can be incorporated into this node's Locator
	 */
	int permanent;
} ZT_InterfaceAddress;

/**
 * Variant type for storing possible path endpoints or peer contact points.
 */
typedef struct
{
	/**
	 * Endpoint type, which determines what field in the union 'a' applies.
	 */
	enum ZT_EndpointType type;

	union {
		/**
		 * Socket address generic buffer
		 */
		struct sockaddr_storage ss;

		/**
		 * Socket address header, for all ZT_ENDPOINT_TYPE_IP types
		 */
		struct sockaddr sa;

		/**
		 * IPv4 address, for all ZT_ENDPOINT_TYPE_IP types if family is AF_INET
		 */
		struct sockaddr_in sa_in;

		/**
		 * IPv6 address, for all ZT_ENDPOINT_TYPE_IP types if family is AF_INET6
		 */
		struct sockaddr_in6 sa_in6;

		/**
		 * MAC address (least significant 48 bites) for ZT_ENDPOINT_TYPE_ETHERNET and other MAC addressed types
		 */
		uint64_t mac;

		/**
		 * ZeroTier node address and identity fingerprint for ZT_ENDPOINT_TYPE_ZEROTIER
		 */
		ZT_Fingerprint fp;
	} value;
} ZT_Endpoint;

/**
 * Network path to a peer
 */
typedef struct
{
	/**
	 * Path endpoint
	 */
	ZT_Endpoint endpoint;

	/**
	 * Time of last send in milliseconds or 0 for never
	 */
	int64_t lastSend;

	/**
	 * Time of last receive in milliseconds or 0 for never
	 */
	int64_t lastReceive;

	/**
	 * Is path alive?
	 */
	int alive;

	/**
	 * Is path preferred?
	 */
	int preferred;
} ZT_Path;

/**
 * Peer information
 */
typedef struct
{
	/**
	 * ZeroTier address (40 bits)
	 */
	uint64_t address;

	/**
	 * Peer identity
	 */
	const ZT_Identity *identity;

	/**
	 * SHA-384 of identity public key(s)
	 */
	ZT_Fingerprint fingerprint;

	/**
	 * Remote major version or -1 if not known
	 */
	int versionMajor;

	/**
	 * Remote minor version or -1 if not known
	 */
	int versionMinor;

	/**
	 * Remote revision or -1 if not known
	 */
	int versionRev;

	/**
	 * Last measured latency in milliseconds or -1 if unknown
	 */
	int latency;

	/**
	 * If non-zero this peer is a root
	 */
	int root;

	/**
	 * Number of networks in which this peer is authenticated
	 */
	unsigned int networkCount;

	/**
	 * Network IDs for networks (array size: networkCount)
	 */
	uint64_t *networks;

	/**
	 * Number of paths (size of paths[])
	 */
	unsigned int pathCount;

	/**
	 * Known network paths to peer (array size: pathCount).
	 *
	 * These are direct paths only. Endpoints can also describe indirect paths,
	 * but those would not appear here. Right now those can only be relaying via
	 * a root.
	 */
	ZT_Path *paths;

	/**
	 * Size of locator in bytes or 0 if none
	 */
	unsigned int locatorSize;

	/**
	 * Serialized locator or NULL if none
	 */
	const void *locator;
} ZT_Peer;

/**
 * List of peers
 */
typedef struct
{
	ZT_Peer *peers;
	unsigned long peerCount;
} ZT_PeerList;

/**
 * ZeroTier core state objects
 */
enum ZT_StateObjectType
{
	/**
	 * Null object -- ignored
	 */
	ZT_STATE_OBJECT_NULL = 0,

	/**
	 * Public address and public key
	 *
	 * Object ID: (none)
	 * Canonical path: <HOME>/identity.public
   * Persistence: required
	 */
	ZT_STATE_OBJECT_IDENTITY_PUBLIC = 1,

	/**
	 * Full identity with secret key
	 *
	 * Object ID: (none)
	 * Canonical path: <HOME>/identity.secret
   * Persistence: required, should be stored with restricted permissions e.g. mode 0600 on *nix
	 */
	ZT_STATE_OBJECT_IDENTITY_SECRET = 2,

	/**
	 * This node's locator
	 *
	 * Object ID: (none)
	 * Canonical path: <HOME>/locator
	 * Persistence: optional
	 */
	ZT_STATE_OBJECT_LOCATOR = 3,

	/**
	 * Peer and related state
	 *
	 * Object ID: [1]address (40 bits, in least significant 64 bits)
	 * Canonical path: <HOME>/peers.d/<ID> (10-digit address)
	 * Persistence: optional, can be cleared at any time
	 */
	ZT_STATE_OBJECT_PEER = 5,

	/**
	 * Network configuration
	 *
	 * Object ID: [1]id (64-bit network ID)
	 * Canonical path: <HOME>/networks.d/<NETWORKID>.conf (16-digit hex ID)
	 * Persistence: required if network memberships should persist
	 */
	ZT_STATE_OBJECT_NETWORK_CONFIG = 6,

	/**
	 * List of certificates, their local trust, and locally added roots
	 *
	 * Object ID: (none)
	 * Canonical path: <HOME>/trust
	 * Persistence: required if root settings should persist
	 */
	ZT_STATE_OBJECT_TRUST_STORE = 7,

	/**
	 * Certificate
	 *
	 * Object ID: [6]serial (384-bit serial packed into 6 uint64_t's)
	 * Canonical path: <HOME>/certs.d/<serial> (96-digit hex serial)
	 */
	ZT_STATE_OBJECT_CERT = 8
};

/**
 * Size of the object ID for peers (in 64-bit uint64_t's)
 */
#define ZT_STATE_OBJECT_PEER_ID_SIZE 1

/**
 * Size of the object ID for network configurations (in 64-bit uint64_t's)
 */
#define ZT_STATE_OBJECT_NETWORK_CONFIG_ID_SIZE 1

/**
 * Size of the object ID for certificates (in 64-bit uint64_t's)
 */
#define ZT_STATE_OBJECT_CERT_ID_SIZE 6

/**
 * An instance of a ZeroTier One node (opaque)
 */
typedef void ZT_Node;

/* ---------------------------------------------------------------------------------------------------------------- */

/**
 * Callback called to update virtual network port configuration
 *
 * This can be called at any time to update the configuration of a virtual
 * network port. The parameter after the network ID specifies whether this
 * port is being brought up, updated, brought down, or permanently deleted.
 *
 * This in turn should be used by the underlying implementation to create
 * and configure tap devices at the OS (or virtual network stack) layer.
 *
 * The supplied config pointer is not guaranteed to remain valid, so make
 * a copy if you want one.
 *
 * This should not call multicastSubscribe() or other network-modifying
 * methods, as this could cause a deadlock in multithreaded or interrupt
 * driven environments.
 */
typedef void (*ZT_VirtualNetworkConfigFunction)(
	ZT_Node *,                             /* Node */
	void *,                                /* User ptr */
	void *,                                /* Thread ptr */
	uint64_t,                              /* Network ID */
	void **,                               /* Modifiable network user PTR */
	enum ZT_VirtualNetworkConfigOperation, /* Config operation */
	const ZT_VirtualNetworkConfig *);      /* Network configuration */

/**
 * Function to send a frame out to a virtual network port
 *
 * Parameters: (1) node, (2) user ptr, (3) network ID, (4) source MAC,
 * (5) destination MAC, (6) ethertype, (7) VLAN ID, (8) frame data,
 * (9) frame length.
 */
typedef void (*ZT_VirtualNetworkFrameFunction)(
	ZT_Node *,                             /* Node */
	void *,                                /* User ptr */
	void *,                                /* Thread ptr */
	uint64_t,                              /* Network ID */
	void **,                               /* Modifiable network user PTR */
	uint64_t,                              /* Source MAC */
	uint64_t,                              /* Destination MAC */
	unsigned int,                          /* Ethernet type */
	unsigned int,                          /* VLAN ID (0 for none) */
	const void *,                          /* Frame data */
	unsigned int);                         /* Frame length */

/**
 * Callback for events
 *
 * Events are generated when the node's status changes in a significant way
 * and on certain non-fatal errors and events of interest. The final void
 * parameter points to event meta-data. The type of event meta-data (and
 * whether it is present at all) is event type dependent. See the comments
 * in the definition of ZT_Event.
 */
typedef void (*ZT_EventCallback)(
	ZT_Node *,                             /* Node */
	void *,                                /* User ptr */
	void *,                                /* Thread ptr */
	enum ZT_Event,                         /* Event type */
	const void *);                         /* Event payload (if applicable) */

/**
 * Callback for storing and/or publishing state information
 *
 * See ZT_StateObjectType docs for information about each state object type
 * and when and if it needs to be persisted.
 *
 * The state object ID's size depends on the object type, and is always
 * in the form of one or more 64-bit unsigned integers. Some object types
 * do not use this field, and for these it may be NULL.
 *
 * An object of length -1 is sent to indicate that an object should be
 * deleted.
 */
typedef void (*ZT_StatePutFunction)(
	ZT_Node *,                             /* Node */
	void *,                                /* User ptr */
	void *,                                /* Thread ptr */
	enum ZT_StateObjectType,               /* State object type */
	const uint64_t *,                      /* State object ID (if applicable) */
	const void *,                          /* State object data */
	int);                                  /* Length of data or -1 to delete */

/**
 * Callback for retrieving stored state information
 *
 * This function should return the number of bytes actually stored to the
 * buffer or -1 if the state object was not found. The buffer itself should
 * be set to point to the data, and the last result parameter must point to
 * a function that will be used to free the buffer when the core is done
 * with it. This is very often just a pointer to free().
 */
typedef int (*ZT_StateGetFunction)(
	ZT_Node *,                             /* Node */
	void *,                                /* User ptr */
	void *,                                /* Thread ptr */
	enum ZT_StateObjectType,               /* State object type */
	const uint64_t *,                      /* State object ID (if applicable) */
	void **,                               /* Result parameter: data */
	void (**)(void *));                    /* Result parameter: data free function */

/**
 * Function to send a ZeroTier packet out over the physical wire (L2/L3)
 *
 * If there is only one local socket, the local socket can be ignored.
 * If the local socket is -1, the packet should be sent out from all
 * bound local sockets or a random bound local socket.
 *
 * If TTL is nonzero, packets should have their IP TTL value set to this
 * value if possible. If this is not possible it is acceptable to ignore
 * this value and send anyway with normal or default TTL.
 *
 * The function must return zero on success and may return any error code
 * on failure. Note that success does not (of course) guarantee packet
 * delivery. It only means that the packet appears to have been sent.
 */
typedef int (*ZT_WirePacketSendFunction)(
	ZT_Node *,                        /* Node */
	void *,                           /* User ptr */
	void *,                           /* Thread ptr */
	int64_t,                          /* Local socket */
	const struct sockaddr_storage *,  /* Remote address */
	const void *,                     /* Packet data */
	unsigned int,                     /* Packet length */
	unsigned int);                    /* TTL or 0 to use default */

/**
 * Function to initiate HTTP requests
 *
 * The supplied HTTP request identifier is an opaque pointer that must
 * be returned via ZT_Node_processHttpResponse(). If this handler is
 * implemented then ZT_Node_processHttpResponse() must be called for
 * each call made by the core to this. This function itself does not
 * return any error code; use processHttpResponse() for that. It may
 * be called directly from inside the implementation of this.
 */
typedef void (*ZT_HTTPRequestFunction)(
	ZT_Node *,                        /* Node */
	void *,                           /* User ptr */
	void *,                           /* Thread ptr */
	void *,                           /* HTTP request identifier */
	const char *,                     /* HTTP method (GET, HEAD, etc.) */
	const char *,                     /* URL */
	const char **,                    /* Header names, NULL terminated */
	const char **,                    /* Header values, NULL terminated */
	const void *,                     /* Request body or NULL if none */
	unsigned int,                     /* Length of request body in bytes */
	unsigned int);                    /* Flags */

/**
 * Function to check whether a path should be used for ZeroTier traffic
 *
 * Parameters:
 *  (1) Node
 *  (2) User pointer
 *  (3) ZeroTier address or 0 for none/any
 *  (4) Full identity or NULL for none/any
 *  (5) Local socket or -1 if unknown
 *  (6) Remote address
 *
 * This function must return nonzero (true) if the path should be used.
 *
 * If no path check function is specified, ZeroTier will still exclude paths
 * that overlap with ZeroTier-assigned and managed IP address blocks. But the
 * use of a path check function is recommended to ensure that recursion does
 * not occur in cases where addresses are assigned by the OS or managed by
 * an out of band mechanism like DHCP. The path check function should examine
 * all configured ZeroTier interfaces and check to ensure that the supplied
 * addresses will not result in ZeroTier traffic being sent over a ZeroTier
 * interface (recursion).
 */
typedef int (*ZT_PathCheckFunction)(
	ZT_Node *,                        /* Node */
	void *,                           /* User ptr */
	void *,                           /* Thread ptr */
	uint64_t,                         /* ZeroTier address */
	const ZT_Identity *,              /* Full identity of node */
	int64_t,                          /* Local socket or -1 if unknown */
	const struct sockaddr_storage *); /* Remote address */

/**
 * Function to get physical addresses for ZeroTier peers
 *
 * Parameters:
 *  (1) Node
 *  (2) User pointer
 *  (3) ZeroTier address (least significant 40 bits)
 *  (4) Identity in string form
 *  (5) Desired address family or -1 for any
 *  (6) Buffer to fill with result
 *
 * If provided this function will be occasionally called to get physical
 * addresses that might be tried to reach a ZeroTier address. It must
 * return a nonzero (true) value if the result buffer has been filled
 * with an address.
 */
typedef int (*ZT_PathLookupFunction)(
	ZT_Node *,                        /* Node */
	void *,                           /* User ptr */
	void *,                           /* Thread ptr */
	uint64_t,                         /* ZeroTier address (40 bits) */
	const ZT_Identity *,              /* Full identity of node */
	int,                              /* Desired ss_family or -1 for any */
	struct sockaddr_storage *);       /* Result buffer */

/* ---------------------------------------------------------------------------------------------------------------- */

/**
 * Structure for configuring ZeroTier core callback functions
 */
struct ZT_Node_Callbacks
{
	/**
	 * REQUIRED: Function to store and/or replicate state objects
	 */
	ZT_StatePutFunction statePutFunction;

	/**
	 * REQUIRED: Function to retrieve state objects from an object store
	 */
	ZT_StateGetFunction stateGetFunction;

	/**
	 * REQUIRED: Function to send packets over the physical wire
	 */
	ZT_WirePacketSendFunction wirePacketSendFunction;

	/**
	 * RECOMMENDED: Function to initiate HTTP requests
	 */
	ZT_HTTPRequestFunction httpRequestFunction;

	/**
	 * REQUIRED: Function to inject frames into a virtual network's TAP
	 */
	ZT_VirtualNetworkFrameFunction virtualNetworkFrameFunction;

	/**
	 * REQUIRED: Function to be called when virtual networks are configured or changed
	 */
	ZT_VirtualNetworkConfigFunction virtualNetworkConfigFunction;

	/**
	 * REQUIRED: Function to be called to notify external code of important events
	 */
	ZT_EventCallback eventCallback;

	/**
	 * OPTIONAL: Function to check whether a given physical path should be used for ZeroTier traffic
	 */
	ZT_PathCheckFunction pathCheckFunction;

	/**
	 * RECOMMENDED: Function to look up paths to ZeroTier nodes
	 */
	ZT_PathLookupFunction pathLookupFunction;
};

/* ---------------------------------------------------------------------------------------------------------------- */

/**
 * Get a buffer for reading data to be passed back into the core via one of the processX() functions
 *
 * The size of the returned buffer is 16384 bytes (ZT_BUF_SIZE).
 *
 * Buffers retrieved with this method MUST be returned to the core via either one of the processX()
 * functions (with isZtBuffer set to true) or freeBuffer(). Buffers should not be freed directly using free().
 *
 * @return Pointer to I/O buffer
 */
ZT_SDK_API void *ZT_getBuffer();

/**
 * Free an unused buffer obtained via getBuffer
 *
 * @param b Buffer to free
 */
ZT_SDK_API void ZT_freeBuffer(void *b);

/**
 * Free a query result buffer
 *
 * Use this to free the return values of listNetworks(), listPeers(), and
 * other query functions that return allocated structures or buffers.
 *
 * @param qr Query result buffer
 */
ZT_SDK_API void ZT_freeQueryResult(void *qr);

/* ---------------------------------------------------------------------------------------------------------------- */

/**
 * Create a new ZeroTier node
 *
 * This will attempt to load its identity via the state get function in the
 * callback struct. If that fails it will generate a new identity and store
 * it. Identity generation can take anywhere from a few hundred milliseconds
 * to a few seconds depending on your CPU speed.
 *
 * @param node Result: pointer is set to new node instance on success
 * @param uptr User pointer to pass to functions/callbacks
 * @param tptr Thread pointer to pass to functions/callbacks resulting from this call
 * @param callbacks Callback function configuration
 * @param now Current clock in milliseconds
 * @return OK (0) or error code if a fatal error condition has occurred
 */
ZT_SDK_API enum ZT_ResultCode ZT_Node_new(
	ZT_Node **node,
	void *uptr,
	void *tptr,
	const struct ZT_Node_Callbacks *callbacks,
	int64_t now);

/**
 * Delete a node and free all resources it consumes
 *
 * If you are using multiple threads, all other threads must be shut down
 * first. This can crash if processXXX() methods are in progress.
 *
 * @param node Node to delete
 * @param tptr Thread pointer to pass to functions/callbacks resulting from this call
 */
ZT_SDK_API void ZT_Node_delete(
	ZT_Node *node,
	void *tptr);

/**
 * Process a packet received from the physical wire
 *
 * @param node Node instance
 * @param tptr Thread pointer to pass to functions/callbacks resulting from this call
 * @param now Current clock in milliseconds
 * @param localSocket Local socket (you can use 0 if only one local socket is bound and ignore this)
 * @param remoteAddress Origin of packet
 * @param packetData Packet data
 * @param packetLength Packet length
 * @param isZtBuffer If non-zero then packetData is a buffer obtained with ZT_getBuffer()
 * @param nextBackgroundTaskDeadline Value/result: set to deadline for next call to processBackgroundTasks()
 * @return OK (0) or error code if a fatal error condition has occurred
 */
ZT_SDK_API enum ZT_ResultCode ZT_Node_processWirePacket(
	ZT_Node *node,
	void *tptr,
	int64_t now,
	int64_t localSocket,
	const struct sockaddr_storage *remoteAddress,
	const void *packetData,
	unsigned int packetLength,
	int isZtBuffer,
	volatile int64_t *nextBackgroundTaskDeadline);

/**
 * Process a frame from a virtual network port (tap)
 *
 * @param node Node instance
 * @param tptr Thread pointer to pass to functions/callbacks resulting from this call
 * @param now Current clock in milliseconds
 * @param nwid ZeroTier 64-bit virtual network ID
 * @param sourceMac Source MAC address (least significant 48 bits)
 * @param destMac Destination MAC address (least significant 48 bits)
 * @param etherType 16-bit Ethernet frame type
 * @param vlanId 10-bit VLAN ID or 0 if none
 * @param frameData Frame payload data
 * @param frameLength Frame payload length
 * @param isZtBuffer If non-zero then packetData is a buffer obtained with ZT_getBuffer()
 * @param nextBackgroundTaskDeadline Value/result: set to deadline for next call to processBackgroundTasks()
 * @return OK (0) or error code if a fatal error condition has occurred
 */
ZT_SDK_API enum ZT_ResultCode ZT_Node_processVirtualNetworkFrame(
	ZT_Node *node,
	void *tptr,
	int64_t now,
	uint64_t nwid,
	uint64_t sourceMac,
	uint64_t destMac,
	unsigned int etherType,
	unsigned int vlanId,
	const void *frameData,
	unsigned int frameLength,
	int isZtBuffer,
	volatile int64_t *nextBackgroundTaskDeadline);

/**
 * Process a response from HTTP requests initiated via API callback
 *
 * @param node Node instance
 * @param tptr Thread pointer to pass to functions/callbacks resulting from this call
 * @param now Current clock in milliseconds
 * @param requestId Opaque pointer provided via the requesting callback
 * @param responseCode HTTP response code (e.g. 200, 500)
 * @param headerNames HTTP header names, terminated by a NULL pointer
 * @param headerValues HTTP header values corresponding with each name
 * @param body Response body or NULL if none
 * @param bodySize Size of response body in bytes
 * @param flags Response flags
 * @return OK (0) or error code if a fatal error condition has occurred
 */
ZT_SDK_API enum ZT_ResultCode ZT_Node_processHTTPResponse(
	ZT_Node *node,
	void *tptr,
	int64_t now,
	void *requestId,
	int responseCode,
	const char **headerNames,
	const char **headerValues,
	const void *body,
	unsigned int bodySize,
	unsigned int flags);

/**
 * Perform periodic background operations
 *
 * @param node Node instance
 * @param tptr Thread pointer to pass to functions/callbacks resulting from this call
 * @param now Current clock in milliseconds
 * @param nextBackgroundTaskDeadline Value/result: set to deadline for next call to processBackgroundTasks()
 * @return OK (0) or error code if a fatal error condition has occurred
 */
ZT_SDK_API enum ZT_ResultCode ZT_Node_processBackgroundTasks(
	ZT_Node *node,
	void *tptr,
	int64_t now,
	volatile int64_t *nextBackgroundTaskDeadline);

/**
 * Join a network
 *
 * This may generate calls to the port config callback before it returns,
 * or these may be differed if a netconf is not available yet.
 *
 * If we are already a member of the network, nothing is done and OK is
 * returned.
 *
 * @param node Node instance
 * @param nwid 64-bit ZeroTier network ID
 * @param fingerprintHash If non-NULL this is the full fingerprint of the controller
 * @param uptr An arbitrary pointer to associate with this network (default: NULL)
 * @param tptr Thread pointer to pass to functions/callbacks resulting from this call
 * @return OK (0) or error code if a fatal error condition has occurred
 */
ZT_SDK_API enum ZT_ResultCode ZT_Node_join(
	ZT_Node *node,
	uint64_t nwid,
	const ZT_Fingerprint *controllerFingerprint,
	void *uptr,
	void *tptr);

/**
 * Leave a network
 *
 * If a port has been configured for this network this will generate a call
 * to the port config callback with a NULL second parameter to indicate that
 * the port is now deleted.
 *
 * The uptr parameter is optional and is NULL by default. If it is not NULL,
 * the pointer it points to is set to this network's uptr on success.
 *
 * @param node Node instance
 * @param nwid 64-bit network ID
 * @param uptr Target pointer is set to uptr (if not NULL)
 * @param tptr Thread pointer to pass to functions/callbacks resulting from this call
 * @return OK (0) or error code if a fatal error condition has occurred
 */
ZT_SDK_API enum ZT_ResultCode ZT_Node_leave(
	ZT_Node *node,
	uint64_t nwid,
	void **uptr,
	void *tptr);

/**
 * Subscribe to an Ethernet multicast group
 *
 * ADI stands for additional distinguishing information. This defaults to zero
 * and is rarely used. Right now its only use is to enable IPv4 ARP to scale,
 * and this must be done.
 *
 * For IPv4 ARP, the implementation must subscribe to 0xffffffffffff (the
 * broadcast address) but with an ADI equal to each IPv4 address in host
 * byte order. This converts ARP from a non-scalable broadcast protocol to
 * a scalable multicast protocol with perfect address specificity.
 *
 * If this is not done, ARP will not work reliably.
 *
 * Multiple calls to subscribe to the same multicast address will have no
 * effect. It is perfectly safe to do this.
 *
 * This does not generate an update call to networkConfigCallback().
 *
 * @param node Node instance
 * @param tptr Thread pointer to pass to functions/callbacks resulting from this call
 * @param nwid 64-bit network ID
 * @param multicastGroup Ethernet multicast or broadcast MAC (least significant 48 bits)
 * @param multicastAdi Multicast ADI (least significant 32 bits only, use 0 if not needed)
 * @return OK (0) or error code if a fatal error condition has occurred
 */
ZT_SDK_API enum ZT_ResultCode ZT_Node_multicastSubscribe(
	ZT_Node *node,
	void *tptr,
	uint64_t nwid,
	uint64_t multicastGroup,
	unsigned long multicastAdi);

/**
 * Unsubscribe from an Ethernet multicast group (or all groups)
 *
 * If multicastGroup is zero (0), this will unsubscribe from all groups. If
 * you are not subscribed to a group this has no effect.
 *
 * This does not generate an update call to networkConfigCallback().
 *
 * @param node Node instance
 * @param nwid 64-bit network ID
 * @param multicastGroup Ethernet multicast or broadcast MAC (least significant 48 bits)
 * @param multicastAdi Multicast ADI (least significant 32 bits only, use 0 if not needed)
 * @return OK (0) or error code if a fatal error condition has occurred
 */
ZT_SDK_API enum ZT_ResultCode ZT_Node_multicastUnsubscribe(
	ZT_Node *node,
	uint64_t nwid,
	uint64_t multicastGroup,
	unsigned long multicastAdi);

/**
 * Designate a peer as a root, adding if not already known
 *
 * ZeroTier does not take possession of the 'id' object. It still must be
 * deleted if it was allocated.
 *
 * @param node Node instance
 * @param tptr Thread pointer to pass to functions/callbacks resulting from this call
 * @param id Identity of root to add
 * @return OK (0) or error code if an error condition has occurred
 */
ZT_SDK_API enum ZT_ResultCode ZT_Node_addRoot(
	ZT_Node *node,
	void *tptr,
	const ZT_Identity *id);

/**
 * Un-designate a peer as a root
 *
 * This doesn't fully remove the peer from the peer list. It just removes
 * its root trust flag. If there is no longer any need to communicate with it
 * it may gradually time out and be removed.
 * 
 * The removeRoot() only takes an address since the identity is by definition
 * already known and pinned.
 *
 * @param node Node instance
 * @param tptr Thread pointer to pass to functions/callbacks resulting from this call
 * @param address ZeroTier address to remove
 * @return OK (0) or error code if an error condition has occurred
 */
ZT_SDK_API enum ZT_ResultCode ZT_Node_removeRoot(
	ZT_Node *node,
	void *tptr,
	const uint64_t address);

/**
 * Get this node's 40-bit ZeroTier address
 *
 * @param node Node instance
 * @return ZeroTier address (least significant 40 bits of 64-bit int)
 */
ZT_SDK_API uint64_t ZT_Node_address(ZT_Node *node);

/**
 * Get this node's identity
 *
 * The identity pointer returned by this function need not and should not be
 * freed with ZT_Identity_delete(). It's valid until the node is deleted.
 *
 * @param node Node instance
 * @return Identity
 */
ZT_SDK_API const ZT_Identity *ZT_Node_identity(ZT_Node *node);

/**
 * Get the status of this node
 *
 * @param node Node instance
 * @param status Buffer to fill with current node status
 */
ZT_SDK_API void ZT_Node_status(
	ZT_Node *node,
	ZT_NodeStatus *status);

/**
 * Get a list of known peer nodes
 *
 * The pointer returned here must be freed with freeQueryResult()
 * when you are done with it.
 *
 * @param node Node instance
 * @return List of known peers or NULL on failure
 */
ZT_SDK_API ZT_PeerList *ZT_Node_peers(ZT_Node *node);

/**
 * Get the status of a virtual network
 *
 * The pointer returned here must be freed with freeQueryResult()
 * when you are done with it.
 *
 * @param node Node instance
 * @param nwid 64-bit network ID
 * @return Network configuration or NULL if we are not a member of this network
 */
ZT_SDK_API ZT_VirtualNetworkConfig *ZT_Node_networkConfig(
	ZT_Node *node,
	uint64_t nwid);

/**
 * Enumerate and get status of all networks
 *
 * @param node Node instance
 * @return List of networks or NULL on failure
 */
ZT_SDK_API ZT_VirtualNetworkList *ZT_Node_networks(ZT_Node *node);

/**
 * Set the network-associated user-defined pointer for a given network
 *
 * This will have no effect if the network ID is not recognized.
 *
 * @param node Node instance
 * @param nwid Network ID
 * @param ptr New network-associated pointer
 */
ZT_SDK_API void ZT_Node_setNetworkUserPtr(
	ZT_Node *node,
	uint64_t nwid,
	void *ptr);

/**
 * Set external interface addresses where this node could be reached
 *
 * @param node Node instance
 * @param addrs Addresses
 * @param addrCount Number of items in addrs[]
 */
ZT_SDK_API void ZT_Node_setInterfaceAddresses(
	ZT_Node *node,
	const ZT_InterfaceAddress *addrs,
	unsigned int addrCount);

/**
 * Add a peer directly by supplying its identity
 * 
 * This does not authorize the peer on a network (only the network's
 * controller can do that) or otherwise give it special privileges. It
 * also doesn't guarantee it will be contacted. It just adds it to the
 * internal peer data set if it is not already present.
 * 
 * @param node Node instance
 * @param tptr Thread pointer to pass to functions/callbacks resulting from this call
 * @param id Identity of peer to add
 * @return OK (0) or error code
 */
ZT_SDK_API enum ZT_ResultCode ZT_Node_addPeer(
	ZT_Node *node,
	void *tptr,
	const ZT_Identity *id);

/**
 * Attempt to contact a peer at an explicit endpoint address.
 * 
 * If the fingerprint structure's hash is all zeroes, the peer is
 * looked up only by address.
 * 
 * This can only fail if the peer was not found.
 * 
 * Note that this can immediately (before this returns) result in
 * calls to the send packet functions supplied to the core.
 * 
 * @param node Node instance
 * @param tptr Thread pointer to pass to functions/callbacks resulting from this call
 * @param fp Fingerprint (or only address)
 * @param endpoint Endpoint
 * @param retries If greater than zero, try this many times
 * @return Boolean: non-zero on success, zero if peer was not found
 */
ZT_SDK_API int ZT_Node_tryPeer(
	ZT_Node *node,
	void *tptr,
	const ZT_Fingerprint *fp,
	const ZT_Endpoint *endpoint,
	int retries);

/**
 * Send a VERB_USER_MESSAGE to another ZeroTier node
 *
 * There is no delivery guarantee here. Failure can occur if the message is
 * too large or if dest is not a valid ZeroTier address.
 *
 * @param node Node instance
 * @param tptr Thread pointer to pass to functions/callbacks resulting from this call
 * @param dest Destination ZeroTier address
 * @param typeId VERB_USER_MESSAGE type ID
 * @param data Payload data to attach to user message
 * @param len Length of data in bytes
 * @return Boolean: non-zero on success, zero on failure
 */
ZT_SDK_API int ZT_Node_sendUserMessage(
	ZT_Node *node,
	void *tptr,
	uint64_t dest,
	uint64_t typeId,
	const void *data,
	unsigned int len);

/**
 * Set a network controller instance for this node
 *
 * Normal nodes should not need to use this. This is for nodes with
 * special compiled-in support for acting as network configuration
 * masters / controllers.
 *
 * The supplied instance must be a C++ object that inherits from the
 * NetworkConfigMaster base class in node/. No type checking is performed,
 * so a pointer to anything else will result in a crash.
 *
 * @param node ZertTier One node
 * @param networkConfigMasterInstance Instance of NetworkConfigMaster C++ class or NULL to disable
 * @return OK (0) or error code if a fatal error condition has occurred
 */
ZT_SDK_API void ZT_Node_setController(
	ZT_Node *node,
	void *networkConfigMasterInstance);

/* ---------------------------------------------------------------------------------------------------------------- */

/**
 * Generate a new identity
 *
 * Due to a small amount of proof of work this can be a time consuming and CPU
 * intensive operation. It takes less than a second on most desktop-class systems
 * but can take longer on e.g. phones.
 *
 * @param type Type of identity to generate
 * @return New identity or NULL on error
 */
ZT_SDK_API ZT_Identity *ZT_Identity_new(enum ZT_IdentityType type);

/**
 * Create a new identity object from a string-serialized identity
 *
 * @param idStr Identity in string format
 * @return Identity object or NULL if the supplied identity string was not valid
 */
ZT_SDK_API ZT_Identity *ZT_Identity_fromString(const char *idStr);

/**
 * Validate this identity
 *
 * This can be slightly time consuming due to address derivation (work) checking.
 *
 * @return Non-zero if identity is valid
 */
ZT_SDK_API int ZT_Identity_validate(const ZT_Identity *id);

/**
 * Sign a data object with this identity
 *
 * The identity must have a private key or this will fail.
 *
 * @param id Identity to use to sign
 * @param data Data to sign
 * @param len Length of data
 * @param signature Buffer to store signature
 * @param signatureBufferLength Length of buffer (must be at least 96 bytes)
 * @return Length of signature in bytes or 0 on failure.
 */
ZT_SDK_API unsigned int ZT_Identity_sign(
	const ZT_Identity *id,
	const void *data,
	unsigned int len,
	void *signature,
	unsigned int signatureBufferLength);

/**
 * Verify a signature
 *
 * @param id Identity to use to verify
 * @param data Data to verify
 * @param len Length of data
 * @param signature Signature to check
 * @param sigLen Length of signature in bytes
 * @return Non-zero if signature is valid
 */
ZT_SDK_API int ZT_Identity_verify(
	const ZT_Identity *id,
	const void *data,
	unsigned int len,
	const void *signature,
	unsigned int sigLen);

/**
 * Get identity type
 *
 * @param id Identity to query
 * @return Identity type code
 */
ZT_SDK_API enum ZT_IdentityType ZT_Identity_type(const ZT_Identity *id);

/**
 * Convert an identity to its string representation
 *
 * @param id Identity to convert
 * @param buf Buffer to store identity (should be at least about 1024 bytes in length)
 * @param capacity Capacity of buffer
 * @param includePrivate If true include the private key if present
 * @return Pointer to buf or NULL on overflow or other error
 */
ZT_SDK_API char *ZT_Identity_toString(
	const ZT_Identity *id,
	char *buf,
	int capacity,
	int includePrivate);

/**
 * Check whether this identity object also holds a private key
 *
 * @param id Identity to query
 * @return Non-zero if a private key is held
 */
ZT_SDK_API int ZT_Identity_hasPrivate(const ZT_Identity *id);

/**
 * Get the ZeroTier address associated with this identity
 *
 * @param id Identity to query
 * @return ZeroTier address (only least significant 40 bits are meaningful, rest will be 0)
 */
ZT_SDK_API uint64_t ZT_Identity_address(const ZT_Identity *id);

/**
 * Get this identity's full fingerprint
 *
 * @param id Identity to query
 * @return Pointer to fingerprint (remains valid as long as identity itself is valid)
 */
ZT_SDK_API const ZT_Fingerprint *ZT_Identity_fingerprint(const ZT_Identity *id);

/**
 * Delete an identity and free associated memory
 *
 * This should only be used with identities created via Identity_new
 * and Identity_fromString().
 *
 * @param id Identity to delete
 */
ZT_SDK_API void ZT_Identity_delete(ZT_Identity *id);

/* ---------------------------------------------------------------------------------------------------------------- */

/**
 * Convert an endpoint to a string
 *
 * @param ep Endpoint structure
 * @param buf Buffer to store string (recommended size: 256)
 * @param capacity Capacity of buffer
 * @return String or NULL on error
 */
ZT_SDK_API char *ZT_Endpoint_toString(
	const ZT_Endpoint *ep,
	char *buf,
	int capacity);

/**
 * Parse an endpoint as a string
 *
 * This will automatically detect IP addresses in IP/port format. If one
 * of these is specified rather than a fully specified endpoint it will be
 * parsed as an IP/UDP endpoint.
 *
 * @param ep Endpoint structure to populate
 * @param str String representation of endpoint
 * @return OK (0) or error code
 */
ZT_SDK_API int ZT_Endpoint_fromString(
	ZT_Endpoint *ep,
	const char *str);

/* ---------------------------------------------------------------------------------------------------------------- */

/**
 * Create and sign a new locator
 *
 * @param ts Locator timestamp
 * @param endpoints List of endpoints to store in locator
 * @param endpointCount Number of endpoints (maximum: 8)
 * @param signer Identity to sign locator (must include private key)
 * @return Locator or NULL on error (too many endpoints or identity does not have private key)
 */
ZT_SDK_API ZT_Locator *ZT_Locator_create(
	int64_t ts,
	const ZT_Endpoint *endpoints,
	unsigned int endpointCount,
	const ZT_Identity *signer);

/**
 * Decode a serialized locator
 *
 * @param data Data to deserialize
 * @param len Length of data
 * @return Locator or NULL if data is not valid
 */
ZT_SDK_API ZT_Locator *ZT_Locator_unmarshal(
	const void *data,
	unsigned int len);

/**
 * Decode a locator from string format
 *
 * @param str String format locator
 * @return Locator or NULL if string is not valid
 */
ZT_SDK_API ZT_Locator *ZT_Locator_fromString(const char *str);

/**
 * Serialize this locator into a buffer
 *
 * @param loc Locator to serialize
 * @param buf Buffer to store bytes
 * @param bufSize Size of buffer in bytes (needs to be at least 2048 bytes in size)
 * @return Number of bytes stored to buf or -1 on error such as buffer too small
 */
ZT_SDK_API int ZT_Locator_marshal(
	const ZT_Locator *loc,
	void *buf,
	unsigned int bufSize);

/**
 * Get this locator in string format
 *
 * @param loc Locator
 * @param buf Buffer to store string
 * @param capacity Capacity of buffer in bytes (recommended size: 4096)
 * @return Pointer to buffer or NULL if an error occurs
 */
ZT_SDK_API char *ZT_Locator_toString(
	const ZT_Locator *loc,
	char *buf,
	int capacity);

/**
 * Get a pointer to the fingerprint of this locator's signer.
 *
 * The returned pointer remains valid as long as the Locator is not deleted.
 *
 * @param loc Locator to query
 * @return Pointer to fingerprint of signer
 */
ZT_SDK_API const ZT_Fingerprint *ZT_Locator_fingerprint(const ZT_Locator *loc);

/**
 * Get a locator's timestamp
 *
 * @param loc Locator to query
 * @return Locator timestamp in milliseconds since epoch
 */
ZT_SDK_API int64_t ZT_Locator_timestamp(const ZT_Locator *loc);

/**
 * Get the number of endpoints in this locator
 *
 * @param loc Locator to query
 * @return Number of endpoints
 */
ZT_SDK_API unsigned int ZT_Locator_endpointCount(const ZT_Locator *loc);

/**
 * Get a pointer to an endpoint in a locator
 *
 * The returned pointer remains valid as long as the Locator is not deleted.
 *
 * @param ep Endpoint number from 0 to 1 - endpointCount()
 * @return Endpoint or NULL if out of bounds
 */
ZT_SDK_API const ZT_Endpoint *ZT_Locator_endpoint(
	const ZT_Locator *loc,
	const unsigned int ep);

/**
 * Verify this locator's signature
 *
 * @param signer Signing identity
 * @return Non-zero if locator is valid
 */
ZT_SDK_API int ZT_Locator_verify(
	const ZT_Locator *loc,
	const ZT_Identity *signer);

/**
 * Delete a locator
 *
 * @param loc Locator to delete
 */
ZT_SDK_API void ZT_Locator_delete(ZT_Locator *loc);

/* ---------------------------------------------------------------------------------------------------------------- */

/**
 * Get ZeroTier core version
 *
 * @param major Result: major version
 * @param minor Result: minor version
 * @param revision Result: revision
 * @param build Result: build number
 */
ZT_SDK_API void ZT_version(
	int *major,
	int *minor,
	int *revision,
	int *build);

/* ---------------------------------------------------------------------------------------------------------------- */

/**
 * Create a new certificate subject unique ID and private key
 *
 * A unique ID is really a public/private key pair.
 *
 * @param type Unique ID type (key pair algorithm)
 * @param uniqueId Unique ID buffer
 * @param uniqueIdSize Value/result: size of buffer
 * @param uniqueIdPrivate Unique ID private key buffer
 * @param uniqueIdPrivateSize Value/result: size of buffer
 * @return OK (0) or error
 */
ZT_SDK_API int ZT_Certificate_newSubjectUniqueId(
	enum ZT_CertificateUniqueIdType type,
	void *uniqueId,
	int *uniqueIdSize,
	void *uniqueIdPrivate,
	int *uniqueIdPrivateSize);

/**
 * Create a new certificate signing request (CSR)
 *
 * A CSR is effectively just an encoded certificate subject.
 * If both uniqueId and uniqueIdPrivate are specified, the subject
 * will be signed with a unique ID. Otherwise these fields are not
 * set. If a unique ID and unique ID signature are present in the
 * supplied subject, these will be ignored.
 *
 * @param subject Subject filled in with fields for CSR
 * @param uniqueId Unique ID or NULL if none
 * @param uniqueIdSize Size of unique ID
 * @param uniqueIdPrivate Unique ID private key or NULL if none
 * @param uniqueIdPrivateSize Size of unique ID private key
 * @param csr Buffer to hold CSR (recommended size: 16384 bytes)
 * @param csrSize Value/result: size of buffer
 * @return OK (0) or error
 */
ZT_SDK_API int ZT_Certificate_newCSR(
	const ZT_Certificate_Subject *subject,
	const void *uniqueId,
	int uniqueIdSize,
	const void *uniqueIdPrivate,
	int uniqueIdPrivateSize,
	void *csr,
	int *csrSize);

/**
 * Sign a CSR to generate a complete certificate
 *
 * @param cert Certificate to sign
 * @param signer Signer identity (must contain secret key)
 * @param signedCert Signed certificate buffer (recommended size: 16384 bytes)
 * @param signedCertSize Value/result: size of buffer
 * @return OK (0) or error
 */
ZT_SDK_API int ZT_Certificate_sign(
	const ZT_Certificate *cert,
	const ZT_Identity *signer,
	void *signedCert,
	int *signedCertSize);

/**
 * Decode a certificate or CSR
 *
 * A CSR is just the encoded subject part of a certificate. Decoding a CSR
 * results in a certificate whose subject is filled in but nothing else.
 *
 * If no error occurs and the pointer at decodedCert is set to non-NULL,
 * the returned certificate must be freed with ZT_Certificate_delete().
 *
 * @param decodedCert Result parameter: target pointer is set to certificate
 * @param cert Certificate or CSR data
 * @param certSize Size of data
 * @param verify If non-zero, verify signatures and structure
 * @return Certificate error, if any
 */
ZT_SDK_API enum ZT_CertificateError ZT_Certificate_decode(
	ZT_Certificate **decodedCert,
	const void *cert,
	int certSize,
	int verify);

/**
 * Free a certificate created with ZT_Certificate_decode()
 *
 * @param cert Certificate to free
 */
ZT_SDK_API void ZT_Certificate_delete(ZT_Certificate *cert);

/* ---------------------------------------------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif