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
 * SyncPluginImpl.h
 * The synchronous implementation of the USB DMX plugin.
 * Copyright (C) 2010 Simon Newton
 */

#ifndef PLUGINS_USBDMX_SYNCPLUGINIMPL_H_
#define PLUGINS_USBDMX_SYNCPLUGINIMPL_H_

#include <libusb.h>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "ola/base/Macro.h"
#include "plugins/usbdmx/LibUsbAdaptor.h"
#include "plugins/usbdmx/PluginImplInterface.h"
#include "plugins/usbdmx/WidgetFactory.h"

namespace ola {

class Device;

namespace plugin {
namespace usbdmx {

/**
 * @brief The legacy implementation.
 *
 * This implementation spawns a thread for each dongle, and then uses
 * synchronous calls to libusb.
 *
 * This does not support hotplug.
 */
class SyncPluginImpl: public PluginImplInterface,  public WidgetObserver {
 public:
  /**
   * @brief Create a new SyncPluginImpl.
   * @param plugin_adaptor The PluginAdaptor to use, ownership is not
   * transferred.
   * @param plugin The parent Plugin object which is used when creating
   * devices.
   * @param debug_level the debug level to use for libusb.
   */
  SyncPluginImpl(PluginAdaptor *plugin_adaptor,
                 Plugin *plugin,
                 unsigned int debug_level);

  ~SyncPluginImpl();

  bool Start();
  bool Stop();

  bool NewWidget(class AnymaWidget *widget);
  bool NewWidget(class EuroliteProWidget *widget);
  bool NewWidget(class ScanlimeFadecandyWidget *widget);
  bool NewWidget(class SunliteWidget *widget);
  bool NewWidget(class VellemanWidget *widget);

  void WidgetRemoved(OLA_UNUSED class AnymaWidget *widget) {}
  void WidgetRemoved(OLA_UNUSED class EuroliteProWidget *widget) {}
  void WidgetRemoved(OLA_UNUSED class ScanlimeFadecandyWidget *widget) {}
  void WidgetRemoved(OLA_UNUSED class SunliteWidget *widget) {}
  void WidgetRemoved(OLA_UNUSED class VellemanWidget *widget) {}

 private:
  typedef std::vector<class WidgetFactory*> WidgetFactories;
  typedef std::map<class Widget*, Device*> WidgetToDeviceMap;

  PluginAdaptor* const m_plugin_adaptor;
  Plugin* const m_plugin;
  const unsigned int m_debug_level;
  SyncronousLibUsbAdaptor m_usb_adaptor;
  WidgetFactories m_widget_factories;

  libusb_context *m_context;

  WidgetToDeviceMap m_devices;
  std::set<std::pair<uint8_t, uint8_t> > m_registered_devices;

  unsigned int ScanForDevices();
  void ReScanForDevices();
  bool CheckDevice(libusb_device *device);

  bool StartAndRegisterDevice(class Widget *widget, Device *device);

  DISALLOW_COPY_AND_ASSIGN(SyncPluginImpl);
};
}  // namespace usbdmx
}  // namespace plugin
}  // namespace ola
#endif  // PLUGINS_USBDMX_SYNCPLUGINIMPL_H_
