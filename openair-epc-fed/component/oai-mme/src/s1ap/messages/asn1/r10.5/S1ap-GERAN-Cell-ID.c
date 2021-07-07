/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "S1AP-IEs"
 * 	found in "S1AP-IEs.asn"
 */

#include "S1ap-GERAN-Cell-ID.h"

static asn_TYPE_member_t asn_MBR_S1ap_GERAN_Cell_ID_1[] = {
    {ATF_NOFLAGS, 0, offsetof(struct S1ap_GERAN_Cell_ID, lAI),
     (ASN_TAG_CLASS_CONTEXT | (0 << 2)), -1, /* IMPLICIT tag at current level */
     &asn_DEF_S1ap_LAI, 0, /* Defer constraints checking to the member type */
     0,                    /* PER is not compiled, use -gen-PER */
     0, "lAI"},
    {ATF_NOFLAGS, 0, offsetof(struct S1ap_GERAN_Cell_ID, rAC),
     (ASN_TAG_CLASS_CONTEXT | (1 << 2)), -1, /* IMPLICIT tag at current level */
     &asn_DEF_S1ap_RAC, 0, /* Defer constraints checking to the member type */
     0,                    /* PER is not compiled, use -gen-PER */
     0, "rAC"},
    {ATF_NOFLAGS, 0, offsetof(struct S1ap_GERAN_Cell_ID, cI),
     (ASN_TAG_CLASS_CONTEXT | (2 << 2)), -1, /* IMPLICIT tag at current level */
     &asn_DEF_S1ap_CI, 0, /* Defer constraints checking to the member type */
     0,                   /* PER is not compiled, use -gen-PER */
     0, "cI"},
    {ATF_POINTER, 1, offsetof(struct S1ap_GERAN_Cell_ID, iE_Extensions),
     (ASN_TAG_CLASS_CONTEXT | (3 << 2)), -1, /* IMPLICIT tag at current level */
     &asn_DEF_S1ap_IE_Extensions,
     0, /* Defer constraints checking to the member type */
     0, /* PER is not compiled, use -gen-PER */
     0, "iE-Extensions"},
};
static ber_tlv_tag_t asn_DEF_S1ap_GERAN_Cell_ID_tags_1[] = {
    (ASN_TAG_CLASS_UNIVERSAL | (16 << 2))};
static asn_TYPE_tag2member_t asn_MAP_S1ap_GERAN_Cell_ID_tag2el_1[] = {
    {(ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0}, /* lAI at 430 */
    {(ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0}, /* rAC at 431 */
    {(ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0}, /* cI at 432 */
    {(ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0}  /* iE-Extensions at 433 */
};
static asn_SEQUENCE_specifics_t asn_SPC_S1ap_GERAN_Cell_ID_specs_1 = {
    sizeof(struct S1ap_GERAN_Cell_ID),
    offsetof(struct S1ap_GERAN_Cell_ID, _asn_ctx),
    asn_MAP_S1ap_GERAN_Cell_ID_tag2el_1,
    4, /* Count of tags in the map */
    0,
    0,
    0, /* Optional elements (not needed) */
    3, /* Start extensions */
    5  /* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_S1ap_GERAN_Cell_ID = {
    "S1ap-GERAN-Cell-ID",
    "S1ap-GERAN-Cell-ID",
    SEQUENCE_free,
    SEQUENCE_print,
    SEQUENCE_constraint,
    SEQUENCE_decode_ber,
    SEQUENCE_encode_der,
    SEQUENCE_decode_xer,
    SEQUENCE_encode_xer,
    0,
    0, /* No UPER support, use "-gen-PER" to enable */
    0,
    0, /* No APER support, use "-gen-PER" to enable */
    SEQUENCE_compare,
    0, /* Use generic outmost tag fetcher */
    asn_DEF_S1ap_GERAN_Cell_ID_tags_1,
    sizeof(asn_DEF_S1ap_GERAN_Cell_ID_tags_1) /
        sizeof(asn_DEF_S1ap_GERAN_Cell_ID_tags_1[0]), /* 1 */
    asn_DEF_S1ap_GERAN_Cell_ID_tags_1,                /* Same as above */
    sizeof(asn_DEF_S1ap_GERAN_Cell_ID_tags_1) /
        sizeof(asn_DEF_S1ap_GERAN_Cell_ID_tags_1[0]), /* 1 */
    0, /* No PER visible constraints */
    asn_MBR_S1ap_GERAN_Cell_ID_1,
    4,                                  /* Elements count */
    &asn_SPC_S1ap_GERAN_Cell_ID_specs_1 /* Additional specs */
};
