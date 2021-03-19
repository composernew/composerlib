/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License (https://opensource.org/licenses/MIT)
=============================================================================*/
#include <elements.hpp>
#include "application.h"

int main(int argc, char* argv[])
{
    // Main window background color
    auto constexpr bkd_color = cycfi::elements::rgba(35, 35, 37, 255);

    // Composer application
    application composer(bkd_color, argc, argv);
    composer.run();

    return 0;
}