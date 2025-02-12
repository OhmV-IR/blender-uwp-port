#include "pch.h"
#include "blenderUWPMain.h"
#include "Common\DirectXHelper.h"
#include "winuser.h"
#include "d3d9helper.h"
#include "d3d11.h"
#include "D2DBaseTypes.h"
#include "dcommon.h"
#include "mainMenu.h"

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
	m_mainMenuRenderer = std::unique_ptr<MainMenuRenderer>(new MainMenuRenderer(m_deviceResources));
	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));
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
		m_fpsTextRenderer->Update(m_timer);
		m_mainMenuRenderer->Update(m_timer);
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
	m_mainMenuRenderer->Render();
	m_fpsTextRenderer->Render();

	return true;
}

void blenderUWPMain::OnDeviceLost()
{
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void blenderUWPMain::OnDeviceRestored()
{
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
