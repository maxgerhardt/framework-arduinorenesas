/*
  ssl_fs_io.cpp
  Copyright (c) 2023 Arduino SA.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"
#include "QSPIFlashBlockDevice.h"
#include "FATFileSystem.h"
#include "ssl_fs_io.h"
#include "ssl_debug.h"

static BlockDevice * get_mbedtls_bd()
{
  static QSPIFlashBlockDevice root(PIN_QSPI_CLK, PIN_QSPI_SS, PIN_QSPI_D0, PIN_QSPI_D1, PIN_QSPI_D2, PIN_QSPI_D3);
  return &root;
}

static FATFileSystem * get_mbedtls_fs()
{
  static FATFileSystem mbedtls_fs("mbedtls");
  return &mbedtls_fs;
}

void mbedtls_fs_init(const char * path)
{
  if(!path) {
    return;
  }
  
  BlockDevice * bd = get_mbedtls_bd();
  FATFileSystem *fs = get_mbedtls_fs();

  if(!fs->mount(bd)) {
    //log_e("Failed to mount mbedtls fs");
  }
}
