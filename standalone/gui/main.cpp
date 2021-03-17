/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License (https://opensource.org/licenses/MIT)
=============================================================================*/
#include <elements.hpp>
#include "application.h"

using namespace cycfi::elements;

int main(int argc, char* argv[])
{
    // Main window background color
    auto constexpr bkd_color = rgba(35, 35, 37, 255);

    // Elements application
    app _app(argc, argv, "Composer", "com.composer.composer");

    // Composer application
    application composer(bkd_color, _app);

    return 0;
}