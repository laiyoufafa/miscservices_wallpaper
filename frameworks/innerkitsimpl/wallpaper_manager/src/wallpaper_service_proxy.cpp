/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "file_deal.h"
#include "iremote_broker.h"
#include "i_wallpaper_service.h"
#include "hilog_wrapper.h"
#include "wallpaper_service_proxy.h"

namespace OHOS {
namespace WallpaperMgrService {
using namespace OHOS::HiviewDFX;

std::vector<RgbaColor> WallpaperServiceProxy::GetColors(int wallpaperType)
{
    std::vector<RgbaColor> Colors;
    MessageParcel data, reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOG_ERROR(" Failed to write parcelable ");
        return Colors;
    }
    if (!data.WriteInt32(wallpaperType)) {
        HILOG_ERROR(" Failed to WriteInt32 ");
        return Colors;
    }

    int32_t result = Remote()->SendRequest(GET_COLORS, data, reply, option);
    if (result != ERR_NONE) {
        HILOG_ERROR(" get colors result = %{public}d ", result);
    }

    int tmpsize = reply.ReadInt32();
    for (int i = 0; i < tmpsize; ++i) {
        RgbaColor colorInfo;
        colorInfo.red = reply.ReadInt32();
        colorInfo.blue = reply.ReadInt32();
        colorInfo.green = reply.ReadInt32();
        colorInfo.alpha = reply.ReadInt32();
        Colors.emplace_back(colorInfo);
    }
    return Colors;
}

std::string WallpaperServiceProxy::getUrl()
{
    return FWReadUrl;
    HILOG_INFO("return FWReadUrl= %{public}s ", FWReadUrl.c_str());
}
bool WallpaperServiceProxy::SetWallpaperByMap(int fd, int wallpaperType, int length)
{
    HILOG_INFO(" SetWallpaperByMap ");
    MessageParcel data, reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOG_ERROR(" Failed to write parcelable ");
        return false;
    }
    if (!data.WriteFileDescriptor(fd)) {
        HILOG_ERROR(" Failed to WriteFileDescriptor ");
        return false;
    }

    if (!data.WriteInt32(wallpaperType)) {
        HILOG_ERROR(" Failed to WriteInt32 ");
        return false;
    }
    if (!data.WriteInt32(length)) {
        HILOG_ERROR(" Failed to WriteInt32 ");
        return false;
    }

    int32_t result = Remote()->SendRequest(SET_WALLPAPER_MAP, data, reply, option);
    if (result != ERR_NONE) {
        HILOG_ERROR(" WallpaperCalbackProxy::SetWallpaperfail, result = %{public}d ", result);
        return false;
    }

    return reply.ReadBool();
}
bool WallpaperServiceProxy::SetWallpaperByFD(int fd, int wallpaperType, int length)
{
    HILOG_INFO(" SetWallpaperByFD ");
    MessageParcel data, reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOG_ERROR(" Failed to write parcelable ");
        return false;
    }
    if (!data.WriteFileDescriptor(fd)) {
        HILOG_ERROR(" Failed to WriteFileDescriptor ");
        return false;
    }
    if (!data.WriteInt32(wallpaperType)) {
        HILOG_ERROR(" Failed to WriteInt32 ");
        return false;
    }
    if (!data.WriteInt32(length)) {
        HILOG_ERROR(" Failed to WriteInt32 ");
        return false;
    }

    int32_t result = Remote()->SendRequest(SET_WALLPAPER_URI_FD, data, reply, option);
    if (result != ERR_NONE) {
        HILOG_ERROR(" WallpaperCalbackProxy::SetWallpaperfail, result = %{public}d ", result);
        return false;
    }
    return reply.ReadBool();
}

IWallpaperService::mapFD WallpaperServiceProxy::GetPixelMap(int wallpaperType)
{
    mapFD mapFd;
    HILOG_INFO(" WallpaperServiceProxy::getPixelMap --> start ");
    MessageParcel data, reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOG_ERROR(" Failed to write parcelable ");
        return mapFd;
    }

    if (!data.WriteInt32(wallpaperType)) {
        HILOG_ERROR(" Failed to WriteInt32 ");
        return mapFd;
    }
    int32_t result = Remote()->SendRequest(GET_PIXELMAP, data, reply, option);
    if (result != ERR_NONE) {
        HILOG_ERROR(" WallpaperServiceProxy::GetPixelMap fail, result = %{public}d ", result);
        return mapFd;
    }
    mapFd.size = reply.ReadInt32();
    mapFd.fd = reply.ReadFileDescriptor();
    return mapFd;
}

int   WallpaperServiceProxy::GetWallpaperId(int wallpaperType)
{
    int iWallpaperId = 1;
    MessageParcel data, reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOG_ERROR(" Failed to write parcelable ");
        return false;
    }

    data.WriteInt32(wallpaperType);
    int32_t result = Remote()->SendRequest(GET_WALLPAPER_ID, data, reply, option);
    if (result != ERR_NONE) {
        HILOG_ERROR(" WallpaperServiceProxy::GetWallpaperId fail, result = %{public}d ", result);
        return -1;
    }

    iWallpaperId = reply.ReadInt32();
    HILOG_INFO(" End => iWallpaperId[%{public}d]", iWallpaperId);
    return iWallpaperId;
}
int   WallpaperServiceProxy::GetWallpaperMinHeight()
{
    int iWallpaperMinHeight = 0;
    MessageParcel data, reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOG_ERROR(" Failed to write parcelable ");
        return 0;
    }

    int32_t result = Remote()->SendRequest(GET_WALLPAPER_MIN_HEIGHT, data, reply, option);
    if (result != ERR_NONE) {
        HILOG_ERROR(" WallpaperServiceProxy::GetWallpaperMinHeight fail, result = %{public}d ", result);
        return 0;
    }

    iWallpaperMinHeight = reply.ReadInt32();
    HILOG_INFO(" End => iWallpaperMinHeight[%{public}d]", iWallpaperMinHeight);
    return iWallpaperMinHeight;
}

int   WallpaperServiceProxy::GetWallpaperMinWidth()
{
    int iWallpaperMinWidth = 0;
    MessageParcel data, reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOG_ERROR(" Failed to write parcelable ");
        return 0;
    }

    int32_t result = Remote()->SendRequest(GET_WALLPAPER_MIN_WIDTH, data, reply, option);
    if (result != ERR_NONE) {
        HILOG_ERROR(" WallpaperServiceProxy::GetWallpaperMinWidth fail, result = %{public}d ", result);
        return 0;
    }

    iWallpaperMinWidth = reply.ReadInt32();
    HILOG_INFO(" End => iWallpaperMinWidth[%{public}d]", iWallpaperMinWidth);
    return iWallpaperMinWidth;
}

bool  WallpaperServiceProxy::IsChangePermitted()
{
    bool bFlag = false;
    MessageParcel data, reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOG_ERROR(" Failed to write parcelable ");
        return false;
    }

    int32_t result = Remote()->SendRequest(IS_CHANGE_PERMITTED, data, reply, option);
    if (result != ERR_NONE) {
        HILOG_ERROR(" WallpaperServiceProxy::IsChangePermitted fail, result = %{public}d ", result);
        return false;
    }

    bFlag = reply.ReadBool();
    return bFlag;
}

bool  WallpaperServiceProxy::IsOperationAllowed()
{
    bool bFlag = false;
    MessageParcel data, reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOG_ERROR(" Failed to write parcelable ");
        return false;
    }

    int32_t result = Remote()->SendRequest(IS_OPERATION_ALLOWED, data, reply, option);
    if (result != ERR_NONE) {
        HILOG_ERROR(" WallpaperServiceProxyIsOperationAllowed fail, result = %{public}d ", result);
        return false;
    }

    bFlag = reply.ReadBool();
    return bFlag;
}

bool  WallpaperServiceProxy::ResetWallpaper(int wallpaperType)
{
    bool bFlag = false;
    MessageParcel data, reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOG_ERROR(" Failed to write parcelable ");
        return false;
    }

    data.WriteInt32(wallpaperType);
    int32_t result = Remote()->SendRequest(RESET_WALLPAPER, data, reply, option);
    if (result != ERR_NONE) {
        HILOG_ERROR(" WallpaperServiceProxy::ResetWallpaper fail, result = %{public}d ", result);
        return false;
    }

    bFlag = reply.ReadBool();
    return bFlag;
}

bool WallpaperServiceProxy::ScreenshotLiveWallpaper(int sacleNumber, OHOS::Media::PixelMap pixelMap)
{
    bool bFlag = false;
    MessageParcel data, reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOG_ERROR(" Failed to write parcelable ");
        return false;
    }

    int32_t result = Remote()->SendRequest(SCREEN_SHOT_LIVE_WALLPAPER, data, reply, option);
    if (result != ERR_NONE) {
        HILOG_ERROR(" WallpaperServiceProxy::ScreenshotLiveWallpaper  fail, result = %{public}d ", result);
        return false;
    }

    bFlag = true;
    return bFlag;
}

bool  WallpaperServiceProxy::On(sptr<IWallpaperColorChangeListener> listener)
{
    HILOG_DEBUG("WallpaperServiceProxy::On in");
    MessageParcel data, reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOG_ERROR(" Failed to write parcelable ");
        return false;
    }
    if (listener == nullptr) {
        HILOG_ERROR("listener is nullptr");
        return false;
    }
    if (!data.WriteParcelable(listener->AsObject())) {
        HILOG_ERROR("write subscribe type or parcel failed.");
        return false;
    }
    int32_t result = Remote()->SendRequest(ON, data, reply, option);
    if (result != ERR_NONE) {
        HILOG_ERROR(" WallpaperServiceProxy::On fail, result = %{public}d ", result);
        return false;
    }

    int32_t status = reply.ReadInt32();
    bool ret = (status == 0) ? true : false;
    HILOG_DEBUG("WallpaperServiceProxy::On out");
    return ret;
}

bool  WallpaperServiceProxy::Off(sptr<IWallpaperColorChangeListener> listener)
{
    HILOG_DEBUG("WallpaperServiceProxy::Off in");
    MessageParcel data, reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOG_ERROR(" Failed to write parcelable ");
        return false;
    }
    if (listener == nullptr) {
        HILOG_ERROR("listener is nullptr");
        return false;
    }
    if (!data.WriteParcelable(listener->AsObject())) {
        HILOG_ERROR("write subscribe type or parcel failed.");
        return false;
    }
    int32_t result = Remote()->SendRequest(OFF, data, reply, option);
    if (result != ERR_NONE) {
        HILOG_ERROR(" WallpaperServiceProxy::Off fail, result = %{public}d ", result);
        return false;
    }

    int32_t status = reply.ReadInt32();
    bool ret = (status == 0) ? true : false;
    HILOG_DEBUG("WallpaperServiceProxy::Off out");
    return ret;
}

bool WallpaperServiceProxy::RegisterWallpaperCallback(const sptr<IWallpaperCallback> callback)
{
    HILOG_DEBUG("WallpaperServiceProxy::RegisterWallpaperCallback in");
    MessageParcel data, reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOG_ERROR(" Failed to write parcelable ");
        return false;
    }
    if (callback == nullptr) {
        HILOG_ERROR("callback is nullptr");
        return false;
    }
    if (!data.WriteRemoteObject(callback->AsObject())) {
        HILOG_ERROR("write subscribe type or parcel failed.");
        return false;
    }
    HILOG_INFO("  Remote()->SendRequest");
    int32_t result = Remote()->SendRequest(REGISTER_CALLBACK, data, reply, option);
    if (result != ERR_NONE) {
        HILOG_ERROR(" WallpaperServiceProxy::REGISTER_CALLBACK fail, result = %{public}d ", result);
        return false;
    }

    int32_t status = reply.ReadInt32();
    bool ret = (status == 0) ? true : false;
    HILOG_DEBUG("WallpaperServiceProxy::REGISTER_CALLBACK out");
    return ret;
}
}
}
