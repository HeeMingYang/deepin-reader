// Copyright 2020 PDFium Authors. All rights reserved.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CONSTANTS_ACCESS_PERMISSIONS_H_
#define CONSTANTS_ACCESS_PERMISSIONS_H_

namespace pdfium {
namespace access_permissions {

// PDF 1.7 spec, table 3.20.
// User access permissions.
constexpr uint32_t kModifyContent = 1 << 3;
constexpr uint32_t kModifyAnnotation = 1 << 5;
constexpr uint32_t kFillForm = 1 << 8;
constexpr uint32_t kExtractForAccessibility = 1 << 9;

}  // namespace access_permissions
}  // namespace pdfium

#endif  // CONSTANTS_ACCESS_PERMISSIONS_H_
