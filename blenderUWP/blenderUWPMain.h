#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"
#include "d2d1.h"
#include "d2d1helper.h"
#include "d2dbasetypes.h"
#include "d2derr.h"
#include "d2d1_1.h"
#include "d2d1_1helper.h"
#include "d2d1effects.h"
#include "d2d1effecthelpers.h"
#include <vector>
#include "TextDisplay.h"
#include "ColorBackground.h"

// Renders Direct2D and 3D content on the screen.
using namespace Windows::Foundation;

namespace blenderUWP
{
	class blenderUWPMain : public DX::IDeviceNotify
	{
	public:
		blenderUWPMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~blenderUWPMain();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		// Rendering loop timer.
		DX::StepTimer m_timer;
		bool m_mainMenuEnabled;
		std::vector<TextDisplay> m_mainMenuTextElements;
		std::unique_ptr<ColorBackground> m_mainMenuBackground;
	};
}