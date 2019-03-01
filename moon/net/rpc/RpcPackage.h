/**
  Copyright 2018, Mugui Zhou. All rights reserved.
  Use of this source code is governed by a BSD-style license 
  that can be found in the License file.

  Author: Mugui Zhou
*/
#ifndef MOON_NET_RPC_RPCPACKAGE_H_
#define MOON_NET_RPC_RPCPACKAGE_H_

#include <stddef.h>
#include <stdint.h>

namespace moon
{
namespace net
{
class Buffer;

class RpcPackage
{
public:
    static size_t getRpcPackageHeadSize();

    bool pack(Buffer &buffer);
    bool pack(Buffer &buffer, const void *data, size_t len);

    size_t decode(RpcPackage &pkg, const void *data, size_t len);
private:
    void packHead(Buffer &buffer);    
private:
    uint32_t mMagic;
    uint32_t mCommand;
    uint16_t mType;
    uint32_t mSequence;
    uint64_t mRequestId;
    uint32_t mBodySize;

};  // ~RpcPackage

}  // ~net   
}  // ~moon
#endif