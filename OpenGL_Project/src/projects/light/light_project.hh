/*************************************************************************
 * Copyright (c) 2016 François Trudel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
*************************************************************************/
#ifndef LIGHT_PROJECT_HH
#define LIGHT_PROJECT_HH

namespace gem { namespace particle {
namespace light_project {
void RegisterProject();
void Init();
void Run();
void Terminate();
} /* namespace light_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: LIGHT_PROJECT_HH */