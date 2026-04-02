/**
* @file Device.cpp
 * @brief Implementation of the Device base class.
 * @author evan
 * @date 2026-03-21
 */

#include "Device.h"

/**
 * @brief Constructs a Device object.
 *
 * Base class constructor; no initialization required at this level.
 * Derived classes are responsible for their own setup.
 */
Device::Device(){}

/**
 * @brief Destructs a Device object.
 *
 * Base class destructor; no cleanup required at this level.
 * Declared to ensure derived class destructors are called correctly
 * when deleting through a base class pointer.
 */
Device::~Device(){}
