//
//  LNExtension.cpp
//  LeapNative
//
//  Created by Wouter Verweirder on 01/02/13.
//  Copyright (c) 2013 Wouter Verweirder. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "LNExtension.h"
#include "LNLeapDevice.h"
#include "LNFREUtilities.h"

extern "C" {
    
    FREObject LeapNative_isSupported(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
		std::cout << "LeapNative_isSupported" << std::endl;
		FREObject isSupported;
		FRENewObjectFromBool(1, &isSupported);
		return isSupported;
	}
    
    FREObject LeapNative_getFrame(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
        leapnative::LNLeapDevice* device;
        FREGetContextNativeData(ctx, (void **) &device);
        return device->getFrame();
    }
    
    FREObject LeapNative_enableGesture(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
        leapnative::LNLeapDevice* device;
        FREGetContextNativeData(ctx, (void **) &device);
        
        int gestureClassType;
        FREGetObjectAsInt32(argv[0], &gestureClassType);
        
        bool gestureEnabled = createBoolFromFREObject(argv[1]);
        
        switch (gestureClassType) {
            case 5:
                device->controller->enableGesture(Gesture::TYPE_SWIPE, gestureEnabled);
                break;
            case 6:
                device->controller->enableGesture(Gesture::TYPE_CIRCLE, gestureEnabled);
                break;
            case 7:
                device->controller->enableGesture(Gesture::TYPE_SCREEN_TAP, gestureEnabled);
                break;
            case 8:
                device->controller->enableGesture(Gesture::TYPE_KEY_TAP, gestureEnabled);
                break;
            default:
                std::cout << "LeapNative_enableGesture: invalid argument" << std::endl;
                break;
        }
        return NULL;
    }
    
    FREObject LeapNative_isGestureEnabled(FREContext ctx, void* funcData, uint32_t argc, FREObject argv[]) {
        leapnative::LNLeapDevice* device;
        FREGetContextNativeData(ctx, (void **) &device);
        
        int gestureClassType;
        FREGetObjectAsInt32(argv[0], &gestureClassType);
        
        bool gestureEnabled;

        switch (gestureClassType) {
            case 5:
                gestureEnabled = device->controller->isGestureEnabled(Gesture::TYPE_SWIPE);
                break;
            case 6:
                gestureEnabled = device->controller->isGestureEnabled(Gesture::TYPE_CIRCLE);
                break;
            case 7:
                gestureEnabled = device->controller->isGestureEnabled(Gesture::TYPE_SCREEN_TAP);
                break;
            case 8:
                gestureEnabled = device->controller->isGestureEnabled(Gesture::TYPE_KEY_TAP);
                break;
            default:
                std::cout << "LeapNative_isGestureEnabled: invalid argument" << std::endl;
                gestureEnabled = false;
                break;
        }
        
        return createFREObjectForBool(gestureEnabled);
    }
    
    FRENamedFunction _Shared_methods[] = {
        { (const uint8_t*) "isSupported", 0, LeapNative_isSupported }
	};
    
	FRENamedFunction _Instance_methods[] = {
  		{ (const uint8_t*) "getFrame", 0, LeapNative_getFrame },
  		{ (const uint8_t*) "enableGesture", 0, LeapNative_enableGesture },
  		{ (const uint8_t*) "isGestureEnabled", 0, LeapNative_isGestureEnabled }
	};
    
    void contextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx, uint32_t* numFunctions, const FRENamedFunction** functions) {
        std::cout << "context initializer" << std::endl;
        if ( 0 == strcmp( (const char*) ctxType, "shared" ) )
		{
			*numFunctions = sizeof( _Shared_methods ) / sizeof( FRENamedFunction );
			*functions = _Shared_methods;
		}
		else
        {
            *numFunctions = sizeof( _Instance_methods ) / sizeof( FRENamedFunction );
            *functions = _Instance_methods;
            
            leapnative::LNLeapDevice* device = new leapnative::LNLeapDevice(ctx);
            FRESetContextNativeData(ctx, (void*) device);
        }
	}
    
	void contextFinalizer(FREContext ctx) {
        leapnative::LNLeapDevice* device;
        FREGetContextNativeData(ctx, (void **) &device);
        if(device != NULL)
        {
            delete device;
        }
        std::cout << "context finalizer" << std::endl;
		return;
	}
    
	void LeapNativeInitializer(void** extData, FREContextInitializer* ctxInitializer, FREContextFinalizer* ctxFinalizer) {
		*ctxInitializer = &contextInitializer;
		*ctxFinalizer = &contextFinalizer;
	}
    
	void LeapNativeFinalizer(void* extData) {
		return;
	}
}