//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// Also available under a BSD-style license. See LICENSE.
//
//===----------------------------------------------------------------------===//

#include "torch-mlir/Dialect/TorchConversion/IR/TorchConversionOps.h"

#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/PatternMatch.h"
#include "torch-mlir/Dialect/Torch/IR/TorchTypes.h"
#include "llvm/ADT/StringMap.h"

using namespace mlir;
using namespace mlir::torch;
using namespace mlir::torch::TorchConversion;
using namespace mlir::torch;

static bool haveSameSizeAndElementType(TensorType lhs, TensorType rhs) {
  if (lhs.hasRank() != rhs.hasRank())
    return false;
  bool sameSize = lhs.hasRank() ? lhs.getShape().equals(rhs.getShape()) : true;
  bool sameElementType = lhs.getElementType() == rhs.getElementType();
  return sameElementType && sameSize;
}

//===----------------------------------------------------------------------===//
// ToBuiltinTensorOp
//===----------------------------------------------------------------------===//

LogicalResult ToBuiltinTensorOp::verify() {
  auto resultType = cast<TensorType>(getResult().getType());
  auto operandType =
      cast<Torch::ValueTensorType>(getOperand().getType()).toBuiltinTensor();
  if (!haveSameSizeAndElementType(resultType, operandType)) {
    return emitError()
           << "operand and result must have the same size and dtype";
  }
  return success();
}

LogicalResult ToBuiltinTensorOp::inferReturnTypes(
    MLIRContext *context, std::optional<Location> location, ValueRange operands,
    DictionaryAttr attributes, OpaqueProperties properties, RegionRange regions,
    SmallVectorImpl<Type> &inferredReturnTypes) {
  auto resultType =
      cast<Torch::ValueTensorType>(operands[0].getType()).toBuiltinTensor();
  if (!resultType)
    return failure();
  inferredReturnTypes.push_back(resultType);
  return success();
}

//===----------------------------------------------------------------------===//
// FromBuiltinTensorOp
//===----------------------------------------------------------------------===//

LogicalResult FromBuiltinTensorOp::verify() {
  auto resultType =
      cast<Torch::ValueTensorType>(getResult().getType()).toBuiltinTensor();
  auto operandType = cast<TensorType>(getOperand().getType());
  if (!haveSameSizeAndElementType(resultType, operandType)) {
    return emitError()
           << "operand and result must have the same size and dtype";
  }
  return success();
}

//===----------------------------------------------------------------------===//
// FromI1Op
//===----------------------------------------------------------------------===//

OpFoldResult FromI1Op::fold(FoldAdaptor adaptor) {
  auto attr = dyn_cast_or_null<mlir::BoolAttr>(adaptor.getOperand());
  if (attr) {
    return attr;
  } else {
    return nullptr;
  }
}

//===----------------------------------------------------------------------===//
// ToI1Op
//===----------------------------------------------------------------------===//

OpFoldResult ToI1Op::fold(FoldAdaptor adaptor) {
  auto attr = dyn_cast_or_null<mlir::BoolAttr>(adaptor.getOperand());
  if (attr) {
    return attr;
  } else {
    return nullptr;
  }
}

//===----------------------------------------------------------------------===//
// FromI64Op
//===----------------------------------------------------------------------===//

OpFoldResult FromI64Op::fold(FoldAdaptor adaptor) {
  auto attr = dyn_cast_or_null<mlir::IntegerAttr>(adaptor.getOperand());
  if (attr) {
    return attr;
  } else {
    return nullptr;
  }
}

//===----------------------------------------------------------------------===//
// ToI64Op
//===----------------------------------------------------------------------===//

OpFoldResult ToI64Op::fold(FoldAdaptor adaptor) {
  auto attr = dyn_cast_or_null<mlir::IntegerAttr>(adaptor.getOperand());
  if (attr) {
    return attr;
  } else {
    return nullptr;
  }
}

//===----------------------------------------------------------------------===//
// ToF64Op
//===----------------------------------------------------------------------===//

OpFoldResult ToF64Op::fold(FoldAdaptor adaptor) {
  auto attr = dyn_cast_or_null<mlir::FloatAttr>(adaptor.getOperand());
  if (attr) {
    return attr;
  } else {
    return nullptr;
  }
}

//===----------------------------------------------------------------------===//
// FromF64Op
//===----------------------------------------------------------------------===//

OpFoldResult FromF64Op::fold(FoldAdaptor adaptor) {
  auto attr = dyn_cast_or_null<mlir::FloatAttr>(adaptor.getOperand());
  if (attr) {
    return attr;
  } else {
    return nullptr;
  }
}

#define GET_OP_CLASSES
#include "torch-mlir/Dialect/TorchConversion/IR/TorchConversionOps.cpp.inc"
