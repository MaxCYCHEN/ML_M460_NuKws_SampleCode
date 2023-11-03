#include <cstdint>
#include <cstddef>
#include "BufAttributes.h"

static const uint8_t g_Model[] ALIGNMENT_ATTRIBUTE =
{
    0x1c, 0x00, 0x00, 0x00, 0x54, 0x46, 0x4c, 0x33, 0x14, 0x00, 0x20, 0x00, 0x1c, 0x00, 0x18, 0x00, 0x14, 0x00, 0x10, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x08, 0x00, 0x04, 0x00, 0x14, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00,
    0xdc, 0x00, 0x00, 0x00, 0x28, 0x08, 0x00, 0x00, 0x38, 0x08, 0x00, 0x00, 0xb8, 0x10, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x82, 0xf7, 0xff, 0xff, 0x0c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00,
    0x3c, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x73, 0x65, 0x72, 0x76, 0x69, 0x6e, 0x67, 0x5f, 0x64, 0x65, 0x66, 0x61,
    0x75, 0x6c, 0x74, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x98, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x34, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x9a, 0xf8, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x69, 0x6e, 0x70, 0x75, 0x74, 0x5f, 0x33, 0x00, 0x02, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0xdc, 0xff, 0xff, 0xff, 0x13, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x43, 0x4f, 0x4e, 0x56,
    0x45, 0x52, 0x53, 0x49, 0x4f, 0x4e, 0x5f, 0x4d, 0x45, 0x54, 0x41, 0x44, 0x41, 0x54, 0x41, 0x00, 0x08, 0x00, 0x0c, 0x00,
    0x08, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00,
    0x6d, 0x69, 0x6e, 0x5f, 0x72, 0x75, 0x6e, 0x74, 0x69, 0x6d, 0x65, 0x5f, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x00,
    0x14, 0x00, 0x00, 0x00, 0x48, 0x07, 0x00, 0x00, 0x40, 0x07, 0x00, 0x00, 0xf0, 0x06, 0x00, 0x00, 0xb8, 0x06, 0x00, 0x00,
    0x98, 0x06, 0x00, 0x00, 0x78, 0x06, 0x00, 0x00, 0x48, 0x06, 0x00, 0x00, 0x38, 0x04, 0x00, 0x00, 0xa8, 0x03, 0x00, 0x00,
    0x58, 0x03, 0x00, 0x00, 0xc8, 0x02, 0x00, 0x00, 0xb8, 0x00, 0x00, 0x00, 0xb0, 0x00, 0x00, 0x00, 0xa8, 0x00, 0x00, 0x00,
    0xa0, 0x00, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x5e, 0xf9, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    0x08, 0x00, 0x0e, 0x00, 0x08, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x06, 0x00, 0x08, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0a, 0x00, 0x10, 0x00, 0x0c, 0x00, 0x08, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x32, 0x2e, 0x39, 0x2e, 0x31, 0x00, 0x00, 0x00,
    0xbe, 0xf9, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x31, 0x2e, 0x31, 0x34, 0x2e, 0x30, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0xf1, 0xff, 0xff, 0x20, 0xf1, 0xff, 0xff, 0x24, 0xf1, 0xff, 0xff,
    0x28, 0xf1, 0xff, 0xff, 0x2c, 0xf1, 0xff, 0xff, 0x30, 0xf1, 0xff, 0xff, 0xf2, 0xf9, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x00, 0x00, 0xfc, 0xba, 0xd8, 0x3e, 0x02, 0x2e, 0xc7, 0x3e, 0x00, 0x62, 0x7e, 0xbb, 0x15, 0x4f, 0x53, 0xbc,
    0xff, 0xda, 0xee, 0xbd, 0x5d, 0xb1, 0xc7, 0xbe, 0xe8, 0x61, 0x37, 0x3e, 0xe8, 0x7a, 0xf4, 0xbe, 0xba, 0x24, 0xbf, 0x3e,
    0x8a, 0x0f, 0xb7, 0xbd, 0x04, 0x9b, 0xa3, 0xbe, 0xb9, 0x03, 0xb5, 0xbe, 0xe8, 0xda, 0x82, 0x3d, 0x1c, 0x14, 0xc4, 0x3e,
    0xb0, 0x07, 0xf4, 0x3e, 0x01, 0x56, 0x20, 0xbe, 0x09, 0x70, 0x0c, 0x3f, 0x87, 0xdb, 0x73, 0xbe, 0xd8, 0x82, 0x00, 0x3e,
    0xdf, 0x7a, 0x8d, 0x3e, 0xac, 0x68, 0xe2, 0xbe, 0x9b, 0x81, 0x30, 0x3e, 0xc0, 0x52, 0xe8, 0x3c, 0xfa, 0x3b, 0x24, 0xbe,
    0x40, 0x8b, 0xca, 0x3e, 0x17, 0x16, 0x26, 0x3f, 0x48, 0xab, 0x70, 0x3e, 0x7f, 0x81, 0x16, 0x3e, 0xb2, 0xa7, 0xe6, 0xbe,
    0x80, 0x90, 0xcf, 0x3d, 0x80, 0x72, 0x1e, 0xbc, 0xf5, 0x08, 0x03, 0xbf, 0x9f, 0xec, 0x36, 0xbf, 0x75, 0x2a, 0xaa, 0x3d,
    0x20, 0x55, 0x13, 0x3d, 0xec, 0x31, 0x1e, 0xbf, 0x94, 0x2f, 0x08, 0xbe, 0x51, 0xec, 0xa0, 0x3e, 0x8c, 0x25, 0xe7, 0xbe,
    0x53, 0x5f, 0x78, 0xbe, 0x3e, 0xc8, 0x44, 0xbf, 0x3b, 0x61, 0x81, 0xbe, 0x5c, 0xef, 0x8f, 0x3e, 0x6c, 0xda, 0x4b, 0x3c,
    0x84, 0xd2, 0x11, 0xbf, 0x9d, 0xf4, 0x82, 0xbe, 0x10, 0xd0, 0xd0, 0xbe, 0x6c, 0xa2, 0x45, 0xbe, 0x69, 0xc1, 0x18, 0xbf,
    0x4b, 0x25, 0x08, 0xbf, 0x80, 0x31, 0x75, 0x3c, 0x54, 0x40, 0xb6, 0xbd, 0xd6, 0x09, 0xf1, 0xbe, 0x10, 0xa8, 0xd7, 0x3d,
    0xf8, 0xb4, 0x22, 0xbe, 0x7d, 0x2f, 0xfc, 0xbd, 0x1e, 0xa8, 0x0c, 0xbf, 0x14, 0x1c, 0x3e, 0xbe, 0x98, 0x33, 0x7e, 0x3e,
    0x0d, 0xd0, 0xe8, 0xbe, 0xdd, 0x31, 0x36, 0xbe, 0x57, 0x85, 0xcf, 0x3e, 0xf0, 0x70, 0x71, 0xbe, 0x88, 0x3b, 0xa3, 0xbe,
    0xff, 0xa3, 0xab, 0xbe, 0xc4, 0xc6, 0xa0, 0xbe, 0xd8, 0xd0, 0x4a, 0x3e, 0x5e, 0x11, 0xb6, 0xbe, 0x03, 0x62, 0x88, 0xbe,
    0x28, 0x2b, 0xd6, 0x3e, 0x00, 0x5b, 0xa7, 0xbe, 0xe2, 0x87, 0xaa, 0xbd, 0x5b, 0xf2, 0xf6, 0xbe, 0x63, 0x4a, 0xb6, 0xbe,
    0xb8, 0xf5, 0xbb, 0xbe, 0x84, 0x02, 0x0c, 0xbe, 0xd4, 0x31, 0x1f, 0xbf, 0x78, 0x41, 0x87, 0xbe, 0x80, 0xee, 0x73, 0xbc,
    0x10, 0x70, 0x30, 0x3e, 0xae, 0xa3, 0x04, 0xbf, 0xc9, 0x31, 0x9b, 0xbd, 0x20, 0x6e, 0x62, 0x3d, 0xc6, 0x3d, 0x0b, 0xbf,
    0x70, 0xcf, 0xef, 0xbd, 0xa0, 0xf9, 0x93, 0xbe, 0x70, 0x7f, 0x54, 0xbe, 0x14, 0x40, 0xfa, 0xbd, 0xaf, 0x49, 0xe0, 0xbe,
    0xdb, 0x35, 0xc7, 0xbe, 0xc8, 0xb7, 0x5c, 0x3e, 0x33, 0x3c, 0x84, 0xbe, 0x5e, 0x1f, 0xe0, 0x3d, 0x24, 0xbc, 0xbb, 0x3e,
    0x68, 0x40, 0x12, 0xbe, 0xaa, 0x2e, 0x08, 0xbf, 0x0a, 0x57, 0x21, 0xbf, 0x4b, 0x69, 0x2b, 0xbf, 0x2c, 0x59, 0xad, 0xbe,
    0xfa, 0x86, 0x49, 0xbd, 0x0a, 0x13, 0x20, 0xbf, 0x6b, 0xed, 0xf5, 0x3e, 0x00, 0xa6, 0x5a, 0x3c, 0xbe, 0xc2, 0x6a, 0xbe,
    0xc3, 0x43, 0x30, 0xbf, 0xea, 0x87, 0x91, 0xbe, 0x18, 0x56, 0x0c, 0x3e, 0xa2, 0x80, 0x4a, 0xbf, 0x98, 0xb9, 0x83, 0xbe,
    0x95, 0xc1, 0xf3, 0x3e, 0x60, 0x05, 0x0e, 0xbe, 0xfc, 0xb3, 0xbf, 0x3c, 0x0e, 0x65, 0x69, 0xbf, 0x33, 0x24, 0x14, 0xbe,
    0xb8, 0x26, 0xa5, 0xbe, 0xf9, 0xb8, 0x36, 0xbf, 0xd7, 0xcb, 0xab, 0xbd, 0xc0, 0x3a, 0xa8, 0xbe, 0x20, 0x7b, 0xdd, 0xbd,
    0x4f, 0xf3, 0x3b, 0xbe, 0xe3, 0xfb, 0x85, 0xbf, 0xbe, 0xf5, 0x01, 0xbf, 0x68, 0x47, 0x06, 0xbe, 0xe5, 0x8e, 0x37, 0xbe,
    0x30, 0xd3, 0x95, 0xbe, 0x0d, 0x60, 0xb7, 0xbe, 0x00, 0x83, 0xd9, 0xbd, 0x6f, 0xae, 0x82, 0xbe, 0xfe, 0xfb, 0xff, 0xff,
    0x04, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x98, 0xaf, 0x88, 0xbe, 0x57, 0x60, 0x0b, 0x3f, 0xe1, 0x03, 0xe7, 0x3e,
    0x8a, 0x21, 0xd1, 0x3e, 0x3b, 0x5d, 0x83, 0x3e, 0x25, 0x7c, 0x9b, 0x3e, 0x1b, 0xb2, 0x22, 0xbf, 0xd5, 0x81, 0xe5, 0x3e,
    0x68, 0x7f, 0xb6, 0xbe, 0x66, 0xc7, 0xf1, 0xbe, 0x5a, 0xb9, 0xac, 0x3e, 0x98, 0x1c, 0xc6, 0xbe, 0xb6, 0x4a, 0x46, 0x3f,
    0xed, 0x84, 0x92, 0x3e, 0x6b, 0x87, 0x44, 0xbd, 0x6d, 0x28, 0x45, 0x3f, 0x22, 0x25, 0x68, 0x3f, 0x1c, 0xaa, 0x2e, 0x3f,
    0xe9, 0x0a, 0x0e, 0x3f, 0x92, 0xd5, 0x46, 0x3e, 0x16, 0xfb, 0xf3, 0x3e, 0xfa, 0x4d, 0x99, 0xbe, 0x2e, 0x7a, 0xf7, 0xbe,
    0xe7, 0xd9, 0x96, 0xbe, 0x58, 0xaa, 0x4e, 0x3e, 0xe3, 0xb8, 0x0f, 0xbf, 0x62, 0x50, 0xb5, 0x3e, 0x70, 0x5b, 0xb9, 0xbe,
    0x3a, 0x3c, 0x6f, 0x3f, 0x95, 0x7b, 0x1e, 0x3f, 0x1f, 0x75, 0xb3, 0x3e, 0xfb, 0x30, 0x2f, 0x3c, 0x8a, 0xfc, 0xff, 0xff,
    0x04, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xd7, 0x79, 0xbc, 0x3e, 0xbe, 0xaf, 0x27, 0xbf, 0x7c, 0xfa, 0xaa, 0x3f,
    0xf1, 0x1b, 0xc9, 0x3e, 0x5b, 0x37, 0xa4, 0x3f, 0x38, 0xff, 0x1d, 0x3f, 0x99, 0x4b, 0x91, 0xbe, 0xb4, 0xb6, 0x32, 0x3f,
    0x96, 0xf4, 0xe6, 0x3d, 0x7a, 0xef, 0x51, 0xbd, 0x80, 0x60, 0x02, 0xbf, 0xda, 0x69, 0x11, 0xbf, 0x42, 0xdf, 0x76, 0x3f,
    0xb1, 0xd0, 0xb7, 0x3e, 0x6c, 0xff, 0x9b, 0xbe, 0x66, 0xeb, 0x63, 0x3f, 0xd6, 0xfc, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x55, 0xf1, 0xd6, 0x3e, 0x5f, 0xd2, 0x80, 0x3c, 0xcf, 0xa0, 0x09, 0xbf, 0x17, 0xac, 0x62, 0x3f,
    0x51, 0x03, 0x38, 0x3f, 0xda, 0xc6, 0xc6, 0x3e, 0x7c, 0xba, 0x0a, 0xbe, 0x4b, 0xd3, 0x3e, 0xbf, 0x35, 0x45, 0xed, 0x3e,
    0x6f, 0x08, 0xbb, 0xbe, 0x28, 0x8d, 0xf2, 0x3e, 0xce, 0x0d, 0x9d, 0xbe, 0xd8, 0x23, 0x34, 0xbe, 0xcf, 0x55, 0x0c, 0xbf,
    0x42, 0x95, 0xed, 0xbe, 0x49, 0x3a, 0xd7, 0xbd, 0xca, 0x14, 0xce, 0x3d, 0x46, 0x41, 0x0e, 0x3e, 0x27, 0xc2, 0x8b, 0x3e,
    0xb1, 0xb4, 0x01, 0x3f, 0x44, 0x80, 0x3d, 0x3f, 0xb0, 0xcc, 0xc9, 0x3d, 0x45, 0x6b, 0x91, 0xbe, 0x22, 0xd0, 0x10, 0xc0,
    0x81, 0xb9, 0x8e, 0x3f, 0xec, 0xf7, 0x78, 0xbe, 0x42, 0x67, 0x1d, 0xbf, 0x4d, 0x4f, 0x2f, 0x3f, 0xc8, 0xa7, 0xb6, 0xbc,
    0x46, 0xb7, 0x74, 0xbe, 0x44, 0x28, 0x01, 0x3e, 0x20, 0x8c, 0xb6, 0xbf, 0x62, 0xfd, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x00, 0x00, 0x07, 0xb1, 0x44, 0x3f, 0x76, 0x69, 0xb8, 0xbe, 0x56, 0x1b, 0x33, 0xbd, 0x81, 0x9a, 0x7b, 0x3f,
    0x28, 0x53, 0xf6, 0x3e, 0x7e, 0x6b, 0xce, 0x3d, 0x9a, 0x52, 0x43, 0xbe, 0xfb, 0x7c, 0xad, 0x3e, 0x38, 0xd4, 0x2e, 0x3d,
    0x72, 0x34, 0xdb, 0xbe, 0x7a, 0xc8, 0x20, 0xbf, 0xc0, 0x5e, 0xc7, 0xbb, 0x77, 0xfc, 0xf7, 0xbe, 0x3b, 0xef, 0x41, 0x3f,
    0xf2, 0x62, 0x77, 0xbf, 0x12, 0x36, 0xe0, 0xbe, 0xdd, 0xf4, 0xb2, 0x3e, 0x37, 0x5d, 0xf5, 0xbe, 0x17, 0xbd, 0xbd, 0x3e,
    0xff, 0x36, 0x37, 0xbe, 0x2a, 0xb4, 0xce, 0xbe, 0x39, 0xe9, 0xa0, 0x3e, 0xd0, 0x0a, 0xd5, 0xbd, 0xb2, 0x0c, 0x1d, 0xbe,
    0xd2, 0xbc, 0xec, 0xbe, 0x24, 0xf9, 0xb2, 0x3e, 0x4b, 0x5c, 0x60, 0x3d, 0xc5, 0xac, 0xef, 0x3c, 0xb3, 0x67, 0xbe, 0x3e,
    0xa9, 0xfb, 0xd4, 0x3e, 0xd0, 0x8e, 0xe3, 0xbe, 0x04, 0x39, 0xb8, 0xbe, 0xe5, 0x2a, 0x05, 0x3e, 0xaf, 0xe2, 0x1e, 0xbe,
    0x45, 0x6f, 0xfa, 0x3d, 0x91, 0xa8, 0x99, 0xbd, 0x2d, 0xb1, 0xb5, 0xbe, 0x8e, 0x4a, 0xb7, 0xbe, 0xb5, 0x08, 0x03, 0xbe,
    0x2d, 0x39, 0xe7, 0xbe, 0x9d, 0xe3, 0x3f, 0xbd, 0x8b, 0x3f, 0x07, 0x3e, 0x48, 0x08, 0x62, 0x3e, 0x71, 0x7b, 0xf0, 0xbe,
    0x0c, 0x53, 0x4d, 0xbe, 0xa9, 0x56, 0x3f, 0x3e, 0xe7, 0x35, 0xd9, 0x3e, 0x23, 0x9d, 0x8c, 0x3e, 0x23, 0x74, 0x40, 0x3e,
    0xc3, 0x65, 0xb3, 0x3c, 0x29, 0x53, 0x42, 0x3f, 0x4c, 0x54, 0x80, 0x3f, 0x88, 0x4a, 0xa4, 0x3e, 0xe5, 0xfe, 0x95, 0xbc,
    0x3b, 0x04, 0x2f, 0x3f, 0xef, 0x4d, 0x11, 0x3f, 0x25, 0x1e, 0x06, 0x3e, 0x1c, 0x15, 0xff, 0xbe, 0x68, 0xdb, 0x3e, 0x3e,
    0x13, 0xb5, 0xc4, 0x3e, 0xe6, 0xde, 0x26, 0xbf, 0x0a, 0x32, 0x3f, 0x3e, 0xbf, 0xf0, 0x05, 0xbf, 0x26, 0xe3, 0x9d, 0x3e,
    0x3a, 0x10, 0xd0, 0x3e, 0x49, 0xcf, 0x20, 0x3f, 0x83, 0x8b, 0x1c, 0x3f, 0x02, 0xea, 0xd7, 0x3e, 0x9c, 0xd0, 0xb4, 0x3e,
    0x20, 0x03, 0x76, 0xbd, 0xe7, 0x84, 0x3d, 0x3f, 0x44, 0x7d, 0x3c, 0x3c, 0xfc, 0xf6, 0x3d, 0xbe, 0x41, 0x13, 0x2a, 0x3d,
    0x38, 0x12, 0xe7, 0x3c, 0x91, 0xcc, 0xf4, 0x3e, 0x16, 0xce, 0xbf, 0x3e, 0xad, 0xb8, 0xff, 0x3e, 0xf3, 0xcc, 0xb6, 0x3c,
    0x22, 0x21, 0x52, 0x3f, 0x04, 0x02, 0xe6, 0x3e, 0xcc, 0xe2, 0xf4, 0xbe, 0xa0, 0x75, 0xd0, 0xbd, 0xdc, 0xe8, 0xbe, 0xbe,
    0x30, 0xd4, 0xde, 0x3d, 0x80, 0xd9, 0x17, 0xbd, 0xb8, 0xf1, 0xc5, 0x3e, 0x5c, 0x30, 0xe5, 0x3e, 0xd8, 0x4b, 0x4e, 0x3e,
    0xe8, 0x69, 0x8c, 0x3e, 0x38, 0x90, 0x9f, 0xbe, 0x28, 0x3b, 0x39, 0xbe, 0x00, 0x32, 0xb0, 0xbc, 0x30, 0x97, 0x35, 0x3e,
    0xc8, 0x20, 0xe3, 0xbe, 0x9c, 0x03, 0xc9, 0xbe, 0x50, 0x78, 0xa8, 0x3d, 0x0c, 0x60, 0xf4, 0xbe, 0x60, 0x14, 0x36, 0xbd,
    0x40, 0xb8, 0xcf, 0x3c, 0x08, 0x2f, 0x45, 0xbe, 0x9c, 0x50, 0x80, 0xbe, 0x70, 0x0b, 0xa5, 0x3e, 0x08, 0x60, 0xf8, 0xbe,
    0x50, 0xc8, 0xac, 0xbd, 0x60, 0x30, 0xc8, 0xbd, 0x70, 0xfe, 0xb9, 0x3e, 0xcc, 0x22, 0xce, 0x3e, 0xa0, 0xb7, 0x51, 0xbd,
    0x24, 0x5f, 0xfb, 0x3e, 0x20, 0xf8, 0x39, 0xbe, 0x40, 0x12, 0x19, 0x3d, 0xfd, 0xb8, 0x28, 0x3f, 0x9e, 0xe5, 0x31, 0x3f,
    0xe7, 0xc5, 0x8b, 0x3f, 0x77, 0xfc, 0x9b, 0x3f, 0xa7, 0x02, 0x8b, 0x3f, 0x5b, 0x4b, 0x0e, 0x3f, 0x6b, 0x2e, 0x7f, 0x3f,
    0x4d, 0x7f, 0x43, 0x3f, 0x5f, 0x94, 0xad, 0x3f, 0x2a, 0x7c, 0x74, 0x3e, 0xe0, 0x9e, 0x10, 0x3f, 0x47, 0x01, 0x49, 0x3f,
    0xb8, 0x7f, 0x6d, 0xbf, 0x82, 0x2a, 0x21, 0xbf, 0x2d, 0x0f, 0x4c, 0xbf, 0x2b, 0x7b, 0x69, 0xbf, 0x6e, 0xff, 0xff, 0xff,
    0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0xec, 0xe0, 0xaa, 0x3e, 0x3c, 0x13, 0x8d, 0xbd, 0xd2, 0x83, 0x21, 0xbd,
    0xbb, 0x81, 0xcd, 0x3e, 0xbd, 0x08, 0xf7, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0xfd, 0x0e, 0xbf,
    0x9a, 0xff, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x22, 0xfb, 0x92, 0x3e, 0x9a, 0x71, 0x0a, 0xbd,
    0xb0, 0xe6, 0x10, 0x3f, 0xae, 0xa9, 0x8a, 0x3e, 0xb6, 0xff, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
    0x01, 0x8d, 0x4d, 0x3e, 0x44, 0x69, 0xc7, 0x3e, 0x93, 0x64, 0x02, 0x3b, 0x86, 0x99, 0x8d, 0x3e, 0xd2, 0xff, 0xff, 0xff,
    0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0xcc, 0x72, 0x44, 0x3e, 0x5d, 0x33, 0xcc, 0x3d, 0x00, 0x00, 0x00, 0x00,
    0x85, 0x53, 0xe6, 0x3d, 0xc9, 0x07, 0x9d, 0x3e, 0xd4, 0xc5, 0xdf, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x3a, 0x61, 0x28, 0x3e,
    0x00, 0x00, 0x06, 0x00, 0x08, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
    0xe3, 0xb8, 0x4d, 0xbe, 0xd5, 0xc6, 0x15, 0x3f, 0x36, 0xb5, 0xf6, 0xbd, 0x04, 0xfd, 0x9b, 0x3d, 0xa3, 0xa7, 0x56, 0xbe,
    0xd6, 0x37, 0x3b, 0xbe, 0x03, 0xe8, 0x85, 0xbe, 0x9a, 0xb3, 0x12, 0xbe, 0xde, 0x46, 0x52, 0xbe, 0xd3, 0xfa, 0x50, 0xbe,
    0xc5, 0x79, 0x83, 0xbe, 0xcd, 0xf1, 0x5d, 0xbe, 0x9e, 0xcc, 0x3b, 0xbe, 0x50, 0x09, 0x46, 0xbe, 0xe5, 0xd7, 0x60, 0xbe,
    0x0d, 0x39, 0x25, 0xbe, 0x94, 0xf7, 0xff, 0xff, 0x98, 0xf7, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x4d, 0x4c, 0x49, 0x52,
    0x20, 0x43, 0x6f, 0x6e, 0x76, 0x65, 0x72, 0x74, 0x65, 0x64, 0x2e, 0x00, 0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0e, 0x00, 0x18, 0x00, 0x14, 0x00, 0x10, 0x00, 0x0c, 0x00, 0x08, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00,
    0x14, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x78, 0x01, 0x00, 0x00, 0x7c, 0x01, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x6d, 0x61, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00,
    0xd8, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0a, 0x00, 0x10, 0x00, 0x0c, 0x00, 0x08, 0x00, 0x04, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x0f, 0x00, 0x00, 0x00, 0x32, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x0c, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00, 0x48, 0xf8, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x0e, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x62, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x52, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01,
    0x01, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x96, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00,
    0x14, 0x00, 0x00, 0x00, 0x86, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xca, 0xff, 0xff, 0xff,
    0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0xba, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x16, 0x00, 0x00, 0x00, 0x10, 0x00, 0x0c, 0x00,
    0x0b, 0x00, 0x04, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0x00, 0x00, 0x00,
    0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x08, 0x00, 0x07, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x01, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
    0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x11, 0x00, 0x00, 0x00, 0x7c, 0x06, 0x00, 0x00, 0x10, 0x06, 0x00, 0x00, 0xa8, 0x05, 0x00, 0x00, 0x50, 0x05, 0x00, 0x00,
    0xf8, 0x04, 0x00, 0x00, 0xa0, 0x04, 0x00, 0x00, 0x54, 0x04, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0xbc, 0x03, 0x00, 0x00,
    0x70, 0x03, 0x00, 0x00, 0x24, 0x03, 0x00, 0x00, 0x90, 0x02, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0x68, 0x01, 0x00, 0x00,
    0xd4, 0x00, 0x00, 0x00, 0x5c, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xdc, 0xf9, 0xff, 0xff, 0x14, 0x00, 0x00, 0x00,
    0x1c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0xc8, 0xf9, 0xff, 0xff, 0x19, 0x00, 0x00, 0x00, 0x53, 0x74, 0x61, 0x74,
    0x65, 0x66, 0x75, 0x6c, 0x50, 0x61, 0x72, 0x74, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x65, 0x64, 0x43, 0x61, 0x6c, 0x6c, 0x3a,
    0x30, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x30, 0xfa, 0xff, 0xff,
    0x14, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x54, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x1c, 0xfa, 0xff, 0xff, 0x3a, 0x00, 0x00, 0x00,
    0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31,
    0x34, 0x2f, 0x4d, 0x61, 0x74, 0x4d, 0x75, 0x6c, 0x3b, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f,
    0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x34, 0x2f, 0x42, 0x69, 0x61, 0x73, 0x41, 0x64, 0x64, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xa4, 0xfa, 0xff, 0xff, 0x14, 0x00, 0x00, 0x00,
    0x1c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00, 0x90, 0xfa, 0xff, 0xff, 0x55, 0x00, 0x00, 0x00, 0x73, 0x65, 0x71, 0x75,
    0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x33, 0x2f, 0x4d, 0x61,
    0x74, 0x4d, 0x75, 0x6c, 0x3b, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65,
    0x6e, 0x73, 0x65, 0x5f, 0x31, 0x33, 0x2f, 0x52, 0x65, 0x6c, 0x75, 0x3b, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69,
    0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x33, 0x2f, 0x42, 0x69, 0x61, 0x73, 0x41, 0x64,
    0x64, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x34, 0xfb, 0xff, 0xff,
    0x14, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00, 0x20, 0xfb, 0xff, 0xff, 0x55, 0x00, 0x00, 0x00,
    0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31,
    0x32, 0x2f, 0x4d, 0x61, 0x74, 0x4d, 0x75, 0x6c, 0x3b, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f,
    0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x32, 0x2f, 0x52, 0x65, 0x6c, 0x75, 0x3b, 0x73, 0x65, 0x71, 0x75,
    0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x32, 0x2f, 0x42, 0x69,
    0x61, 0x73, 0x41, 0x64, 0x64, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0xc4, 0xfb, 0xff, 0xff, 0x14, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00,
    0x70, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x04, 0x00, 0x00, 0x00, 0xb0, 0xfb, 0xff, 0xff,
    0x55, 0x00, 0x00, 0x00, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e,
    0x73, 0x65, 0x5f, 0x31, 0x31, 0x2f, 0x4d, 0x61, 0x74, 0x4d, 0x75, 0x6c, 0x3b, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74,
    0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x31, 0x2f, 0x52, 0x65, 0x6c, 0x75, 0x3b,
    0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31,
    0x31, 0x2f, 0x42, 0x69, 0x61, 0x73, 0x41, 0x64, 0x64, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x54, 0xfc, 0xff, 0xff, 0x14, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x08, 0x00, 0x00, 0x00,
    0x40, 0xfc, 0xff, 0xff, 0x55, 0x00, 0x00, 0x00, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32,
    0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x30, 0x2f, 0x4d, 0x61, 0x74, 0x4d, 0x75, 0x6c, 0x3b, 0x73, 0x65, 0x71,
    0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x30, 0x2f, 0x52,
    0x65, 0x6c, 0x75, 0x3b, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e,
    0x73, 0x65, 0x5f, 0x31, 0x30, 0x2f, 0x42, 0x69, 0x61, 0x73, 0x41, 0x64, 0x64, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x46, 0xfd, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
    0x0b, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0xc0, 0xfc, 0xff, 0xff, 0x1c, 0x00, 0x00, 0x00, 0x73, 0x65, 0x71, 0x75,
    0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x34, 0x2f, 0x4d, 0x61,
    0x74, 0x4d, 0x75, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x8e, 0xfd, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00,
    0x08, 0xfd, 0xff, 0xff, 0x1c, 0x00, 0x00, 0x00, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32,
    0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x33, 0x2f, 0x4d, 0x61, 0x74, 0x4d, 0x75, 0x6c, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xd6, 0xfd, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0x50, 0xfd, 0xff, 0xff, 0x1c, 0x00, 0x00, 0x00,
    0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31,
    0x32, 0x2f, 0x4d, 0x61, 0x74, 0x4d, 0x75, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x1e, 0xfe, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x2c, 0x00, 0x00, 0x00, 0x98, 0xfd, 0xff, 0xff, 0x1c, 0x00, 0x00, 0x00, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69,
    0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x31, 0x2f, 0x4d, 0x61, 0x74, 0x4d, 0x75, 0x6c,
    0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x66, 0xfe, 0xff, 0xff,
    0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00, 0xe0, 0xfd, 0xff, 0xff,
    0x1c, 0x00, 0x00, 0x00, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e,
    0x73, 0x65, 0x5f, 0x31, 0x30, 0x2f, 0x4d, 0x61, 0x74, 0x4d, 0x75, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xae, 0xfe, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x28, 0xfe, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x73, 0x65, 0x71, 0x75,
    0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x30, 0x2f, 0x42, 0x69,
    0x61, 0x73, 0x41, 0x64, 0x64, 0x2f, 0x52, 0x65, 0x61, 0x64, 0x56, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6c, 0x65, 0x4f, 0x70,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x02, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x7c, 0xfe, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00,
    0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31,
    0x31, 0x2f, 0x42, 0x69, 0x61, 0x73, 0x41, 0x64, 0x64, 0x2f, 0x52, 0x65, 0x61, 0x64, 0x56, 0x61, 0x72, 0x69, 0x61, 0x62,
    0x6c, 0x65, 0x4f, 0x70, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x56, 0xff, 0xff, 0xff,
    0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0xd0, 0xfe, 0xff, 0xff,
    0x2c, 0x00, 0x00, 0x00, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32, 0x2f, 0x64, 0x65, 0x6e,
    0x73, 0x65, 0x5f, 0x31, 0x32, 0x2f, 0x42, 0x69, 0x61, 0x73, 0x41, 0x64, 0x64, 0x2f, 0x52, 0x65, 0x61, 0x64, 0x56, 0x61,
    0x72, 0x69, 0x61, 0x62, 0x6c, 0x65, 0x4f, 0x70, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0xaa, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00,
    0x24, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32,
    0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x33, 0x2f, 0x42, 0x69, 0x61, 0x73, 0x41, 0x64, 0x64, 0x2f, 0x52, 0x65,
    0x61, 0x64, 0x56, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6c, 0x65, 0x4f, 0x70, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x14, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x08, 0x00, 0x04, 0x00,
    0x0e, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00,
    0x88, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x74, 0x69, 0x61, 0x6c, 0x5f, 0x32,
    0x2f, 0x64, 0x65, 0x6e, 0x73, 0x65, 0x5f, 0x31, 0x34, 0x2f, 0x42, 0x69, 0x61, 0x73, 0x41, 0x64, 0x64, 0x2f, 0x52, 0x65,
    0x61, 0x64, 0x56, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6c, 0x65, 0x4f, 0x70, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00, 0x14, 0x00, 0x18, 0x00, 0x14, 0x00, 0x00, 0x00, 0x10, 0x00, 0x0c, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x00, 0x14, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x10, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x73, 0x65, 0x72, 0x76, 0x69, 0x6e, 0x67, 0x5f,
    0x64, 0x65, 0x66, 0x61, 0x75, 0x6c, 0x74, 0x5f, 0x69, 0x6e, 0x70, 0x75, 0x74, 0x5f, 0x33, 0x3a, 0x30, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0xf4, 0xff, 0xff, 0xff, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x0c, 0x00, 0x0c, 0x00,
    0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09,
};


const uint8_t *GetModelPointer()
{
    return g_Model;
}

size_t GetModelLen()
{
    return sizeof(g_Model);
}


const float GetThreshold()
{
    return 0.00683703925460577;
}

const float GetMaxValTrain()
{
    return 3158.1043774;
}

const uint8_t GetFrameLenSample()
{
    return 300;
}

const float GetMinValTrain()
{
    return -0.619863934954322;
}
