/* used to remember the names of the posix attributes in AD */
/* see the rfc2307 & sfu nss backends */

struct posix_schema {
	char *posix_homedir_attr;
	char *posix_shell_attr;
	char *posix_uidnumber_attr;
	char *posix_gidnumber_attr;
	char *posix_gecos_attr;
	char *posix_uid_attr;
};

/* ldap attribute oids (Services for Unix 3.0, 3.5) */
#define ADS_ATTR_SFU_UIDNUMBER_OID 	"1.2.840.113556.1.6.18.1.310"
#define ADS_ATTR_SFU_GIDNUMBER_OID 	"1.2.840.113556.1.6.18.1.311"
#define ADS_ATTR_SFU_HOMEDIR_OID 	"1.2.840.113556.1.6.18.1.344"
#define ADS_ATTR_SFU_SHELL_OID 		"1.2.840.113556.1.6.18.1.312"
#define ADS_ATTR_SFU_GECOS_OID 		"1.2.840.113556.1.6.18.1.337"
#define ADS_ATTR_SFU_UID_OID            "1.2.840.113556.1.6.18.1.309"

/* ldap attribute oids (Services for Unix 2.0) */
#define ADS_ATTR_SFU20_UIDNUMBER_OID	"1.2.840.113556.1.4.7000.187.70"
#define ADS_ATTR_SFU20_GIDNUMBER_OID	"1.2.840.113556.1.4.7000.187.71"
#define ADS_ATTR_SFU20_HOMEDIR_OID	"1.2.840.113556.1.4.7000.187.106"
#define ADS_ATTR_SFU20_SHELL_OID	"1.2.840.113556.1.4.7000.187.72"
#define ADS_ATTR_SFU20_GECOS_OID 	"1.2.840.113556.1.4.7000.187.97"
#define ADS_ATTR_SFU20_UID_OID          "1.2.840.113556.1.4.7000.187.102"


/* ldap attribute oids (RFC2307) */
#define ADS_ATTR_RFC2307_UIDNUMBER_OID	"1.3.6.1.1.1.1.0"
#define ADS_ATTR_RFC2307_GIDNUMBER_OID	"1.3.6.1.1.1.1.1"
#define ADS_ATTR_RFC2307_HOMEDIR_OID	"1.3.6.1.1.1.1.3"
#define ADS_ATTR_RFC2307_SHELL_OID	"1.3.6.1.1.1.1.4"
#define ADS_ATTR_RFC2307_GECOS_OID	"1.3.6.1.1.1.1.2"
#define ADS_ATTR_RFC2307_UID_OID        "0.9.2342.19200300.100.1.1"

enum wb_posix_mapping {
	WB_POSIX_MAP_UNKNOWN    = -1,
	WB_POSIX_MAP_TEMPLATE 	= 0,
	WB_POSIX_MAP_SFU 	= 1,
	WB_POSIX_MAP_SFU20 	= 2,
	WB_POSIX_MAP_RFC2307 	= 3,
	WB_POSIX_MAP_UNIXINFO	= 4
};

/* The following definitions come from libads/ldap_schema.c  */

ADS_STATUS ads_get_attrnames_by_oids(ADS_STRUCT *ads, TALLOC_CTX *mem_ctx,
				     const char *schema_path,
				     const char **OIDs, size_t num_OIDs,
				     char ***OIDs_out, char ***names, size_t *count);
const char *ads_get_attrname_by_guid(ADS_STRUCT *ads,
				     const char *schema_path,
				     TALLOC_CTX *mem_ctx,
				     const struct GUID *schema_guid);
const char *ads_get_attrname_by_oid(ADS_STRUCT *ads, const char *schema_path, TALLOC_CTX *mem_ctx, const char * OID);
ADS_STATUS ads_schema_path(ADS_STRUCT *ads, TALLOC_CTX *mem_ctx, char **schema_path);
ADS_STATUS ads_check_posix_schema_mapping(TALLOC_CTX *mem_ctx,
					  ADS_STRUCT *ads,
					  enum wb_posix_mapping map_type,
					  struct posix_schema **s ) ;