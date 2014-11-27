/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * FadecandyWidget.h
 * The synchronous and asynchronous Fadecandy widgets.
 * Copyright (C) 2014 Simon Newton
 */

#ifndef PLUGINS_USBDMX_FADECANDYWIDGET_H_
#define PLUGINS_USBDMX_FADECANDYWIDGET_H_

#include <libusb.h>
#include <memory>
#include <string>

#include "ola/DmxBuffer.h"
#include "ola/base/Macro.h"
#include "ola/thread/Mutex.h"
#include "plugins/usbdmx/Widget.h"

namespace ola {
namespace plugin {
namespace usbdmx {

/**
 * @brief The interface for the Fadecandy Widgets.
 *
 * Fadecandy devices have 8 physical ports. Each port can drive 64 RGB pixels.
 * Ideally this means we'd model each Fadecandy port as an OLA port, but that
 * introduces syncronization issues, since the underlying protocol models all 8
 * ports as a flat pixel array. For now we just expose the first 170 pixels.
 *
 * See https://github.com/scanlime/fadecandy/blob/master/README.md for more
 * information on Fadecandy devices.
 */
class ScanlimeFadecandyWidget: public BaseWidget {
 public:
  ScanlimeFadecandyWidget(LibUsbAdaptor *adaptor,
                          const std::string &serial)
      : BaseWidget(adaptor),
        m_serial(serial) {
  }

  /**
   * @brief Get the serial number of this widget.
   * @returns The serial number of the widget.
   */
  std::string SerialNumber() const {
    return m_serial;
  }

 private:
  std::string m_serial;
};

/**
 * @brief An Fadecandy widget that uses synchronous libusb operations.
 *
 * Internally this spawns a new thread to avoid blocking SendDMX() calls.
 */
class SynchronousFadecandyWidget: public ScanlimeFadecandyWidget {
 public:
  /**
   * @brief Create a new SynchronousFadecandyWidget.
   * @param adaptor the LibUsbAdaptor to use.
   * @param usb_device the libusb_device to use for the widget.
   * @param serial the serial number of the widget.
   */
  SynchronousFadecandyWidget(LibUsbAdaptor *adaptor,
                             libusb_device *usb_device,
                             const std::string &serial);

  bool Init();

  bool SendDMX(const DmxBuffer &buffer);

 private:
  libusb_device* const m_usb_device;
  std::auto_ptr<class FadecandyThreadedSender> m_sender;

  DISALLOW_COPY_AND_ASSIGN(SynchronousFadecandyWidget);
};

/**
 * @brief An Fadecandy widget that uses asynchronous libusb operations.
 */
class AsynchronousFadecandyWidget : public ScanlimeFadecandyWidget {
 public:
  /**
   * @brief Create a new AsynchronousFadecandyWidget.
   * @param adaptor the LibUsbAdaptor to use.
   * @param usb_device the libusb_device to use for the widget.
   * @param serial the serial number of the widget.
   */
  AsynchronousFadecandyWidget(LibUsbAdaptor *adaptor,
                              libusb_device *usb_device,
                              const std::string &serial);

  bool Init();

  bool SendDMX(const DmxBuffer &buffer);

 private:
  std::auto_ptr<class FadecandyAsyncUsbSender> m_sender;

  DISALLOW_COPY_AND_ASSIGN(AsynchronousFadecandyWidget);
};
}  // namespace usbdmx
}  // namespace plugin
}  // namespace ola
#endif  // PLUGINS_USBDMX_FADECANDYWIDGET_H_
