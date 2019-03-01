/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/

#include <moon/net/Buffer.h>
#include <moon/net/rpc/RpcPackage.h>

#include <assert.h>
#include <algorithm>

namespace moon
{
namespace net
{
static const size_t kMaxHeadSize = 128;
static const size_t kPkgHeadSize = RpcPackage::getRpcPackageHeadSize();

static size_t getBytes(const char *src, char *buffer, size_t count)
{
    if ( (buffer == NULL) || (count == 0) ) {
        return 0;
    }
    
    std::copy(src, src + count, buffer);
    return count;
}

size_t RpcPackage::getRpcPackageHeadSize()
{
    size_t size = 0;
    RpcPackage pkg;
    size += sizeof(pkg.mMagic);
    size += sizeof(pkg.mCommand);
    size += sizeof(pkg.mType);
    size += sizeof(pkg.mSequence);
    size += sizeof(pkg.mRequestId);
    size += sizeof(pkg.mBodySize);
    
    assert(size < kMaxHeadSize);
    return size;
}


bool RpcPackage::pack(Buffer &buffer)
{
    mBodySize = static_cast<uint32_t>(buffer.readableBytes());
    const size_t totalSize = mBodySize + kPkgHeadSize;
    
    Buffer outBuffer;
    outBuffer.ensureWritableBytes(totalSize);
    
    packHead(outBuffer);
    outBuffer.append(buffer.data(), buffer.readableBytes());
    buffer.swap(outBuffer);

    return true;
}

bool RpcPackage::pack(Buffer &buffer, const void *data, size_t len)
{
    if (data == NULL || len == 0) {
        return false;
    }

    packHead(buffer);
    buffer.append(static_cast<const char *>(data), len);
    return true;
}

void RpcPackage::packHead(Buffer &buffer)
{
    buffer.append(reinterpret_cast<const char *>(&mMagic), sizeof(mMagic));
    buffer.append(reinterpret_cast<const char *>(&mCommand), sizeof(mCommand));
    buffer.append(reinterpret_cast<const char *>(&mType), sizeof(mType));
    buffer.append(reinterpret_cast<const char *>(&mSequence), sizeof(mSequence));
    buffer.append(reinterpret_cast<const char *>(&mRequestId), sizeof(mRequestId));
    buffer.append(reinterpret_cast<const char *>(&mBodySize), sizeof(mBodySize));
}

size_t RpcPackage::decode(RpcPackage &pkg, const void *data, size_t len)
{
    if ( (data == NULL) || (len == 0) ) {
        return -1;
    }
    
    if (len < kPkgHeadSize) {
        return 0;
    }
    
    size_t index = 0;
    index += getBytes(static_cast<const char *>(data) + index, reinterpret_cast<char *>(&pkg.mMagic), sizeof(pkg.mMagic));
    index += getBytes(static_cast<const char *>(data) + index, reinterpret_cast<char *>(&pkg.mCommand), sizeof(pkg.mCommand));
    index += getBytes(static_cast<const char *>(data) + index, reinterpret_cast<char *>(&pkg.mType), sizeof(pkg.mType));
    index += getBytes(static_cast<const char *>(data) + index, reinterpret_cast<char *>(&pkg.mSequence), sizeof(pkg.mSequence));
    index += getBytes(static_cast<const char *>(data) + index, reinterpret_cast<char *>(&pkg.mRequestId), sizeof(pkg.mRequestId));
    index += getBytes(static_cast<const char *>(data) + index, reinterpret_cast<char *>(&pkg.mBodySize), sizeof(pkg.mBodySize));

    return kPkgHeadSize + pkg.mBodySize;
}




}  // ~net   
}  // ~moon