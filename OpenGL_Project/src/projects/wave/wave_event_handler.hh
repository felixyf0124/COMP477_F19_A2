#pragma once
#ifndef WAVE_EVENT_HANDLER_HH
#define WAVE_EVENT_HANDLER_HH

#include <memory>
#include "graphic_contexts/graphic_context.hh"
#include "projects\wave\obstacle_collision.hh"

namespace gem { namespace particle {
namespace wave_project {
namespace event_handler {
void Init(const std::shared_ptr<GraphicContext>& a_pCtxt,
	const std::shared_ptr<ObstacleCollision>& a_pObstacleHandle);
void Terminate();
void Update();
} /* namespace event_handler*/
} /* namespace wave_project */
} /* namespace particle */
} /* namespace gem */

#endif /* end of include guard: WAVE_EVENT_HANDLER_HH */