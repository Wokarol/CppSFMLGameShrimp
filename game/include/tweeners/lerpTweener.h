#pragma once

#include <tweener.h>
#include <console.h>
#include <utils/mathUtils.h>

namespace wok
{
	template <class T>
	class LerpTweener : public Tweener
	{
		std::function<void(T)> setter;
		std::function<T()> getter;
		float t = 0;
		float speed;
		T start, end;

	public:
		LerpTweener(ActorHandle<Actor> actor,
			std::function<T()> getter, std::function<void(T)> setter,
			T target, float duration
		) :
			Tweener(actor),
			getter(getter), setter(setter),
			speed(1.f / duration),
			start(getter()), end(target)
		{ }

		virtual void tween(const GameClock& time) override
		{
			t += time.delta * speed;
			if (t < 1)
			{
				setter(m::lerp(start, end, t));
			}
			else
			{
				setter(end);
				kill();
			}
		}
	};
}
