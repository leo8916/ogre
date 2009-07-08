/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/

#include "OgreStableHeaders.h"
#include "OgreLodStrategyManager.h"

#include "OgreException.h"
#include "OgreDistanceLodStrategy.h"
#include "OgrePixelCountLodStrategy.h"

namespace Ogre {
    //-----------------------------------------------------------------------
    template<> LodStrategyManager* Singleton<LodStrategyManager>::ms_Singleton = 0;
    LodStrategyManager* LodStrategyManager::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    LodStrategyManager& LodStrategyManager::getSingleton(void)
    {
        assert( ms_Singleton );  return ( *ms_Singleton );
    }
    //-----------------------------------------------------------------------
    LodStrategyManager::LodStrategyManager()
    {
        // Add default (distance) strategy
        LodStrategy *distanceStrategy = OGRE_NEW DistanceLodStrategy();
        addStrategy(distanceStrategy);

        // Add new pixel-count strategy
        LodStrategy *pixelCountStrategy = OGRE_NEW PixelCountLodStrategy();
        addStrategy(pixelCountStrategy);

        // Set the default strategy
        setDefaultStrategy(distanceStrategy);
    }
    //-----------------------------------------------------------------------
    LodStrategyManager::~LodStrategyManager()
    {
        // Destroy all strategies and clear the map
        removeAllStrategies();
    }
    //-----------------------------------------------------------------------
    void LodStrategyManager::addStrategy(LodStrategy *strategy)
    {
        // Check for invalid strategy name
        if (strategy->getName() == "default")
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Lod strategy name must not be \"default\".", "LodStrategyManager::addStrategy");

        // Insert the strategy into the map with its name as the key
        mStrategies.insert(std::make_pair(strategy->getName(), strategy));
    }
    //-----------------------------------------------------------------------
    LodStrategy *LodStrategyManager::removeStrategy(const String& name)
    {
        // Find strategy with specified name
        StrategyMap::iterator it = mStrategies.find(name);

        // If not found, return null
        if (it == mStrategies.end())
            return 0;

        // Otherwise, erase the strategy from the map
        mStrategies.erase(it);

        // Return the strategy that was removed
        return it->second;
    }
    //-----------------------------------------------------------------------
    void LodStrategyManager::removeAllStrategies()
    {
        // Get beginning iterator
        for (StrategyMap::iterator it = mStrategies.begin(); it != mStrategies.end(); ++it)
		{
			OGRE_DELETE it->second;
		}
		mStrategies.clear();

    }
    //-----------------------------------------------------------------------
    LodStrategy *LodStrategyManager::getStrategy(const String& name)
    {
        // If name is "default", return the default strategy instead of performing a lookup
        if (name == "default")
            return getDefaultStrategy();

        // Find strategy with specified name
        StrategyMap::iterator it = mStrategies.find(name);

        // If not found, return null
        if (it == mStrategies.end())
            return 0;

        // Otherwise, return the strategy
        return it->second;
    }
    //-----------------------------------------------------------------------
    void LodStrategyManager::setDefaultStrategy(LodStrategy *strategy)
    {
        mDefaultStrategy = strategy;
    }
    //-----------------------------------------------------------------------
    void LodStrategyManager::setDefaultStrategy(const String& name)
    {
        // Lookup by name and set default strategy
        setDefaultStrategy(getStrategy(name));
    }
    //-----------------------------------------------------------------------
    LodStrategy *LodStrategyManager::getDefaultStrategy()
    {
        return mDefaultStrategy;
    }
    //-----------------------------------------------------------------------
    MapIterator<LodStrategyManager::StrategyMap> LodStrategyManager::getIterator()
    {
        // Construct map iterator from strategy map and return
        return MapIterator<StrategyMap>(mStrategies);
    }
    //-----------------------------------------------------------------------

}