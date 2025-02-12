#include "pch.h"
#include "blenderUWPMain.h"
#include "Common\DirectXHelper.h"
#include "winuser.h"
#include "d3d9helper.h"
#include "d3d11.h"
#include "D2DBaseTypes.h"
#include "dcommon.h"
#include "TextDisplay.h"

using namespace blenderUWP;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
blenderUWPMain::blenderUWPMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);
	Size screenSize = m_deviceResources->GetLogicalSize();
	m_mainMenuEnabled = true;
	m_mainMenuTextElements = std::vector<TextDisplay>();
	m_mainMenuTextElements.push_back(TextDisplay(deviceResources, std::wstring(L"Blender UWP port"), DWRITE_PARAGRAPH_ALIGNMENT_CENTER, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STRETCH_NORMAL,
		32.0f, D2D1::ColorF(D2D1::ColorF::BlueViolet), DWRITE_TEXT_ALIGNMENT_CENTER, screenSize.Width / 2, 125.0f, 250.0f, 125.0f));
}

blenderUWPMain::~blenderUWPMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void blenderUWPMain::CreateWindowSizeDependentResources() 
{

}

// Updates the application state once per frame.
void blenderUWPMain::Update() 
{
	// Update scene objects.
	m_timer.Tick([&]()
	{
		if (m_mainMenuEnabled) {
			for (int i = 0; i < m_mainMenuTextElements.size(); i++) {
				m_mainMenuTextElements[i].Update();
			}
		}
	});
}


bool blenderUWPMain::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());
	if (m_mainMenuEnabled) {
		for (int i = 0; i < m_mainMenuTextElements.size(); i++) {
			m_mainMenuTextElements[i].Render();
		}
	}
	return true;
}

void blenderUWPMain::OnDeviceLost()
{
	for (int i = 0; i < m_mainMenuTextElements.size(); i++) {
		m_mainMenuTextElements[i].ReleaseDeviceDependentResources();
	}
}

// Notifies renderers that device resources may now be recreated.
void blenderUWPMain::OnDeviceRestored()
{
	for (int i = 0; i < m_mainMenuTextElements.size(); i++) {
		m_mainMenuTextElements[i].CreateDeviceDependentResources();
	}
	CreateWindowSizeDependentResources();
}
