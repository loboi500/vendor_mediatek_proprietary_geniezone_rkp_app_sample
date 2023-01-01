#include <rkp_app_manifest.h>

rkp_app_manifest_t RKP_APP_MANIFEST_ATTRS rkp_app_manifest =
{
	/* d2c50855-6f2b-57ee-2db6-7c49944f30a0 */
	{ 0xd2c50855, 0x6f2b, 0x57ee,
	{ 0x2d, 0xb6, 0x7c, 0x49, 0x94, 0x4f, 0x30, 0xa0 } },

	/* optional configuration options here */
	//rkp configuration
	{
	RKP_CONFIG_ENTRY(0x33880000),//for rkp entry
	RKP_CONFIG_NS_ENTRY(0), //for rkp ns entry. if 0, gz set va(entry) = pa
	},
};

