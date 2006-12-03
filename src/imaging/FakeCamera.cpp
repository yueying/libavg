//
//  libavg - Media Playback Engine. 
//  Copyright (C) 2003-2006 Ulrich von Zadow
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  Current versions can be found at www.libavg.de
//

#include "FakeCamera.h"

#include "../graphics/Pixel8.h"
#include "../graphics/Filterfill.h"
#include "../graphics/Filterfillrect.h"
#include "../graphics/Filtergrayscale.h"

#include "../base/TimeSource.h"
#include "../base/Exception.h"
#include "../base/Logger.h"

#include <Magick++.h>

#include <sstream>

using namespace std;

namespace avg {

const IntPoint IMAGE_SIZE = IntPoint(40,30);

FakeCamera::FakeCamera()
    : m_pBmpQ(new std::queue<BitmapPtr>()),
      m_bIsOpen(false)
{
    for (int i=0; i<6; ++i) {
        stringstream s;
        s << "../imaging/testimages/Blob" << i << ".png";
        try {
            BitmapPtr pBmp (new Bitmap(s.str()));
            FilterGrayscale().applyInPlace(pBmp); 
            m_pBmpQ->push(pBmp);
        } catch (Exception& ex) {
            AVG_TRACE(Logger::ERROR, ex.GetStr());
            throw;
        } catch (Magick::Exception & ex) {
            AVG_TRACE(Logger::ERROR, ex.what());
            throw;
        }
    }
}

FakeCamera::~FakeCamera()
{
}

void FakeCamera::open()
{
    m_bIsOpen = true;
}

void FakeCamera::close()
{
    m_bIsOpen = false;
}


IntPoint FakeCamera::getImgSize()
{
    return IMAGE_SIZE;
}

BitmapPtr FakeCamera::getImage(bool bWait)
{
    if (!m_bIsOpen) {
        return BitmapPtr();
    } else {
        TimeSource::get()->msleep(100);
        BitmapPtr pBmp = m_pBmpQ->front();
        if (m_pBmpQ->size() > 1) {
            m_pBmpQ->pop(); 
        }
        return pBmp; 
    }
}

bool FakeCamera::isCameraAvailable()
{
    return true;
}


const string& FakeCamera::getDevice() const
{
    static string sDeviceName = "FakeCamera";
    return sDeviceName;
}

double FakeCamera::getFrameRate() const
{
    return 60;
}

const string& FakeCamera::getMode() const
{
    static string sMode = "FakeCamera";
    return sMode;
}


unsigned int FakeCamera::getFeature(const std::string& sFeature) const
{
    return 0;
}

void FakeCamera::setFeature(const std::string& sFeature, int Value)
{
}

}




