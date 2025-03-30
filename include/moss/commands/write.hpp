#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <moss/core/contex.hpp>
#include <moss/commands/primitives.hpp>


namespace moss {

/////////////////////
//// -- Query -- ////
/////////////////////
/*
So a dynamic view, a runtime view, is provided in the
atlas and pool statments. So the only compile time
thing about this is what components to get.

Contex::WRITE permissions is only required here,
as actually reading with DynamicView is impossible,
and is only provided by the app to the system.

This is still a WRITE command, but yet the type of
query. It still has *some* templates, so it's not
fully dynamic.
*/


} // moss
