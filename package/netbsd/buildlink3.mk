# $NetBSD$

BUILDLINK_TREE+=	katie

.if !defined(KATIE_BUILDLINK3_MK)
KATIE_BUILDLINK3_MK:=

BUILDLINK_API_DEPENDS.katie+=	katie>=4.9.0.20200114
BUILDLINK_PKGSRCDIR.katie?=	../../devel/katie
.endif	# KATIE_BUILDLINK3_MK

BUILDLINK_TREE+=	-katie