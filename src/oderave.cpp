// Copyright (C) 2006-2009 Rosen Diankov (rdiankov@cs.cmu.edu)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "plugindefs.h"

//#include "odecollision.h"
#include "odephysics.h"
#include "odecontroller.h"
#include "torquecontroller.h"

#include <openrave/plugin.h>

static std::list< OpenRAVE::UserDataPtr >* s_listRegisteredReaders = NULL; ///< have to make it a pointer in order to prevent static object destruction from taking precedence
InterfaceBasePtr CreateInterfaceValidated(InterfaceType type, const std::string& interfacename, std::istream& sinput, EnvironmentBasePtr penv)
{
    if( !s_listRegisteredReaders ) {
        s_listRegisteredReaders = new list< OpenRAVE::UserDataPtr >();
        s_listRegisteredReaders->push_back(RaveRegisterXMLReader(OpenRAVE::PT_PhysicsEngine,"odeproperties_rob",ODEPhysicsEngine::CreateXMLReader));
    }

    switch(type) {
    //case OpenRAVE::PT_CollisionChecker:
        //if( interfacename == "ode_rob")
            //return InterfaceBasePtr(new ODECollisionChecker(penv));
        //break;
    case OpenRAVE::PT_PhysicsEngine:
        if( interfacename == "ode_rob" )
            return InterfaceBasePtr(new ODEPhysicsEngine(penv));
        break;
    case OpenRAVE::PT_Controller:
        if( interfacename == "odevelocity_rob")
            return InterfaceBasePtr(new ODEVelocityController(penv));
        else if( interfacename == "torquecontroller")
            return InterfaceBasePtr(new ODETorqueController(penv));
        break;
    default:
        break;
    }

    return InterfaceBasePtr();
}

void GetPluginAttributesValidated(PLUGININFO& info)
{
    info.interfacenames[OpenRAVE::PT_PhysicsEngine].push_back("ode_rob");
    info.interfacenames[OpenRAVE::PT_Controller].push_back("odevelocity_rob");
    info.interfacenames[OpenRAVE::PT_Controller].push_back("torquecontroller");
}

OPENRAVE_PLUGIN_API void DestroyPlugin()
{
    delete s_listRegisteredReaders;
    s_listRegisteredReaders = NULL;
}
