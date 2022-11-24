#ifndef _dxrr
#define _dxrr
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <d3dx10math.h>
#include "TerrenoRR.h"
#include "Camara.h"
#include "SkyDome.h"
#include "Billboard.h"
#include "ModeloRR.h"
#include "XACT3Util.h"
#include <sstream>
#include "Text.h"

class DXRR{	

private:
	int ancho;
	int alto;
public:	
	HINSTANCE hInstance;
	HWND hWnd;

	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* d3dContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* backBufferTarget;

	ID3D11Texture2D* depthTexture;
	ID3D11DepthStencilView* depthStencilView;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* depthStencilDisabledState;

	ID3D11BlendState *alphaBlendState, *commonBlendState;

	int frameBillboard;

	//TERRENO
	TerrenoRR *terreno;

	//SKYDOME
	SkyDome *skydome;
	BillboardRR *billboard;
	Camara *camara;

	//MODELOS
	ModeloRR* model;
	ModeloRR* vehiculo;
	ModeloRR* llave;
	ModeloRR* pinos;
	ModeloRR* cesped;
	ModeloRR* banca;
	ModeloRR* pelota;
	ModeloRR* juego;
	ModeloRR* roca;

	//MODELO ESCENARIO
	ModeloRR* laberinto;


	//Test Model
	ModeloRR* plano;
	
	
	float izqder;
	float arriaba;
	float vel, vel2;
	bool breakpoint;
	vector2 uv1[32];
	vector2 uv2[32];
	vector2 uv3[32];
	vector2 uv4[32];
	
	                                   //Tipo texto
	                                   Text* Coordenadas;

	XACTINDEX cueIndex;
	CXACT3Util m_XACT3;

	bool camaraTipo;
	float rotCam;
	
	//void CollisionX(float, float , float , float , bool colision, int move);
	//void CollisionZ(float P1, float P2, float P3, float P4, bool colision, int move);


    DXRR(HWND hWnd, int Ancho, int Alto)
	{
		breakpoint = false;
		frameBillboard = 0;
		ancho = Ancho;
		alto = Alto;
		driverType = D3D_DRIVER_TYPE_NULL;
		featureLevel = D3D_FEATURE_LEVEL_11_0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
		IniciaD3D(hWnd);
		izqder = 0;
		arriaba = 0;
		billCargaFuego();
		camara = new Camara(D3DXVECTOR3(0,80,6), D3DXVECTOR3(0,80,0), D3DXVECTOR3(0,1,0), Ancho, Alto);
		terreno = new TerrenoRR(300, 300, d3dDevice, d3dContext);
		
		//SKYDOME
		skydome = new SkyDome(32, 32, 100.0f, &d3dDevice, &d3dContext, L"Skydome.jpg");

		//BILLBOARD
		billboard = new BillboardRR(L"Assets/Billboards/fuego-anim.png",L"Assets/Billboards/fuego-anim-normal.png", d3dDevice, d3dContext, 5);

		//MODELOS
		//model = new ModeloRR(d3dDevice, d3dContext, "Assets/Cofre/Cofre.obj", L"Assets/Cofre/Cofre-color.png", L"Assets/Cofre/Cofre-spec.png", 0, 0);
		llave = new ModeloRR(d3dDevice, d3dContext, "Assets/Llave/Llave.obj", L"Assets/Llave/Llave Color.jpg", L"Assets/Llave/Llave Specular.jpg", 0, 0);
		pinos = new ModeloRR(d3dDevice, d3dContext, "Assets/Pinos/Pinos.obj", L"Assets/Pinos/Pinos Color.png", L"Assets/Pinos/Pinos Specular.jpg", 0, 0);
		pelota = new ModeloRR(d3dDevice, d3dContext, "Assets/Pelota/Pelota.obj", L"Assets/Pelota/Pelota Color.png", L"Assets/Pelota/Pelota Specular.jpg", 0, 0);
		roca = new ModeloRR(d3dDevice, d3dContext, "Assets/Rocas/Roca.obj", L"Assets/Rocas/Roca Color.jpg", L"Assets/Rocas/Roca Specular.jpg", 0, 0);
		juego = new ModeloRR(d3dDevice, d3dContext, "Assets/Juego/Juego.obj", L"Assets/Juego/Juego Color.jpg", L"Assets/Juego/Juego Specular.jpg", 0, 0);
		//cesped = new ModeloRR(d3dDevice, d3dContext, "Assets/Cesped/Cesped.obj", L"Assets/Cesped/Cesped Color.png", L"Assets/Cesped/Cesped Specular.jpg", 0, 0);
		banca = new ModeloRR(d3dDevice, d3dContext, "Assets/Banca/Banca.obj", L"Assets/Banca/Banca Color.png", L"Assets/Banca/Banca Specular.png", 0, 0);
		//vehiculo = new ModeloRR(d3dDevice, d3dContext, "Assets/Auto/Cheep.obj", L"Assets/Auto/Cheep.jpg", L"Assets/Auto/CheepSpec.jpg", 0, 0);

		//MODELO ESCENARIO PRINCIPAL
		laberinto = new ModeloRR(d3dDevice, d3dContext, "Assets/Laberinto/Laberinto.obj", L"Assets/Laberinto/Laberinto Color.jpg", L"Assets/Laberinto/Laberinto Specular.jpg", 0, 0);

		//TEST MODEL
		//plano = new ModeloRR(d3dDevice, d3dContext, "Assets/Plano/Plano.obj", L"Assets/Cofre/Cofre-color.png", L"Assets/Cofre/Cofre-spec.png", 0, 0);

		//Camaras
		camaraTipo = true;
		rotCam = 300.0f;
		
		    //--Monse-- Texto
		Coordenadas = new Text(d3dDevice, d3dContext, 3.6, 1.2, L"Assets/GUI/font.jpg", XMFLOAT4(0.5f, 0.6f, 0.8f, 1.0f));
		

	}

	~DXRR()
	{
		LiberaD3D();
		m_XACT3.Terminate();
	}
	
	bool IniciaD3D(HWND hWnd)
	{
		this->hInstance = hInstance;
		this->hWnd = hWnd;

		//obtiene el ancho y alto de la ventana donde se dibuja
		RECT dimensions;
		GetClientRect(hWnd, &dimensions);
		unsigned int width = dimensions.right - dimensions.left;
		unsigned int heigth = dimensions.bottom - dimensions.top;

		//Las formas en como la pc puede ejecutar el DX11, la mas rapida es D3D_DRIVER_TYPE_HARDWARE pero solo se puede usar cuando lo soporte el hardware
		//otra opcion es D3D_DRIVER_TYPE_WARP que emula el DX11 en los equipos que no lo soportan
		//la opcion menos recomendada es D3D_DRIVER_TYPE_SOFTWARE, es la mas lenta y solo es util cuando se libera una version de DX que no sea soportada por hardware
		D3D_DRIVER_TYPE driverTypes[]=
		{
			D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_SOFTWARE
		};
		unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

		//La version de DX que utilizara, en este caso el DX11
		D3D_FEATURE_LEVEL featureLevels[]=
		{
			D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
		};
		unsigned int totalFeaturesLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = heigth;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hWnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		HRESULT result;
		unsigned int driver = 0, creationFlags = 0;
		for(driver = 0; driver<totalDriverTypes; driver++)
		{
			result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0,
				creationFlags, featureLevels, totalFeaturesLevels, 
				D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
				&d3dDevice, &featureLevel, &d3dContext);

			if(SUCCEEDED(result))
			{
				driverType = driverTypes[driver];
				break;
			}
		}

		if(FAILED(result))
		{

			//Error al crear el Direct3D device
			return false;
		}
		
		ID3D11Texture2D* backBufferTexture;
		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
		if(FAILED(result))
		{
			//"Error al crear el swapChainBuffer
			return false;
		}

		result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);
		if(backBufferTexture)
			backBufferTexture->Release();

		if(FAILED(result))
		{
			//Error al crear el renderTargetView
			return false;
		}


		D3D11_VIEWPORT viewport;
		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)heigth;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		d3dContext->RSSetViewports(1, &viewport);

		D3D11_TEXTURE2D_DESC depthTexDesc;
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
		depthTexDesc.Width = width;
		depthTexDesc.Height = heigth;
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = 1;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		
		result = d3dDevice->CreateTexture2D(&depthTexDesc, NULL, &depthTexture);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear la DepthTexture", MB_OK);
			return false;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = depthTexDesc.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		result = d3dDevice->CreateDepthStencilView(depthTexture, &descDSV, &depthStencilView);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el depth stencil target view", MB_OK);
			return false;
		}

		d3dContext->OMSetRenderTargets(1, &backBufferTarget, depthStencilView);

		return true;			
		
	}

	void LiberaD3D(void)
	{
		if(depthTexture)
			depthTexture->Release();
		if(depthStencilView)
			depthStencilView->Release();
		if(backBufferTarget)
			backBufferTarget->Release();
		if(swapChain)
			swapChain->Release();
		if(d3dContext)
			d3dContext->Release();
		if(d3dDevice)
			d3dDevice->Release();

		depthTexture = 0;
		depthStencilView = 0;
		d3dDevice = 0;
		d3dContext = 0;
		swapChain = 0;
		backBufferTarget = 0;
	}
	
	void Render(void)
	{
		rotCam += izqder;
		float sphere[3] = { 0,0,0 };
		float prevPos[3] = { camara->posCam.x, camara->posCam.z, camara->posCam.z };
		static float angle = 0.0f;
		angle += 0.005;
		if (angle >= 360) angle = 0.0f;
		bool collide = false;
		if (d3dContext == 0)
			return;

		float clearColor[4] = { 0, 0, 0, 1.0f };
		d3dContext->ClearRenderTargetView(backBufferTarget, clearColor);
		d3dContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		camara->posCam.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 4.5;
		camara->posCam3P.y = terreno->Superficie(camara->posCam.x, camara->posCam.z) + 6;
		camara->UpdateCam(vel, arriaba, izqder);

		skydome->Update(camara->vista, camara->proyeccion);

		float camPosXZ[2] = { camara->posCam.x, camara->posCam.z };

		//Temporizador
		// -----La variable tiempo sería de tipo Text*
		// ----Se crea un puntero arriba
		//tiempo->DrawText(-0.95, 0.95, "Tiempo: " + tiempo->Time(segundos), 0.015);



		//Ubicación tiempo real
		stringstream ss;
		ss << camara->posCam.x;
		Coordenadas->DrawText(-0.95, 0.95, "X: " + ss.str(), 0.01);
		ss.str(std::string());
		ss << camara->posCam.z;
		Coordenadas->DrawText(-0.95, 0.85, "Z: " + ss.str(), 0.01);

		//DrawText(-0.9,);




		TurnOffDepth();
		skydome->Render(camara->posCam);
		TurnOnDepth();
		terreno->Draw(camara->vista, camara->proyeccion);
		//TurnOnAlphaBlending();
		billboard->Draw(camara->vista, camara->proyeccion, camara->posCam,
			-11, -78, 4, 5, uv1, uv2, uv3, uv4, frameBillboard);

		//TurnOffAlphaBlending();

		//--------------------------------------------------------MODELOS---------------------------------------
		                      //model->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, camaraTipo, false);
		                      llave->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, camaraTipo, false);
		                      pinos->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, camaraTipo, false);
		                      
		                     
		                      
		                      pelota->Draw(camara->vista, camara->proyeccion, terreno->Superficie(pelota->getPosX(), pelota->getPosZ()), camara->posCam, 10.0f, 0, 'A', 1, camaraTipo, false);
		                      roca->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, camaraTipo, false);
		                      juego->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, camaraTipo, false);
		                      //cesped->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, camaraTipo, false);
		                      banca->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, camaraTipo, false);
		                      //TEST MODEL
		                      //plano->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, camaraTipo, false);
		                      
		                      //MODELO ESCENARIO PRINCIPAL
		                      laberinto->Draw(camara->vista, camara->proyeccion, terreno->Superficie(100, 20), camara->posCam, 10.0f, 0, 'A', 1, camaraTipo, false);
		                      
		                      //vehiculo->setPosX(camara->hdveo.x);
		                      //vehiculo->setPosZ(camara->hdveo.z);
		                      //vehiculo->Draw(camara->vista, camara->proyeccion, terreno->Superficie(vehiculo->getPosX(), vehiculo->getPosZ()) + 2.5, camara->posCam, rotCam + XM_PI, 0, 'Y', 1, camaraTipo, true);
		                      
		                      
	


		//--------------------------------------------------COLISIONES------------------------------
		
					 //ESFERICAS--Chinga tu madre Hugo
					//if (isPointInsideSphere(camara->getPos(), pelota->getSphere(5)))
					//{
					// //MessageBox(hWnd, L"Colision", L"Advertencia", MB_OK);
					// camara->posCam = camara->pastPosCam;
					//}

                             //LINEALES
		                     //***Collision(PX1, PX2, Pz1, Pz2, bool, int);***
	                             
	                             	//Colisión de una piedra cerca del laberinto
	                             //	CollisionX(-139, -97, -19, true, 1);
	                             //
	                             //	//Unos pinos por el laberinto
	                             //	CollisionX(-139, -135, -29, true, 1);
	                             //
	                             //	//Otra roca enorme
	                             //	CollisionX(-149, -100, -82, true, 1);
	                             
                             #pragma region Las colisiones de la entrada frente al juego, que por extraña razón dejaron de jalar bien, aiuda
	                   //Colisiones en X
	                    CollisionX(-34, 68, 68, 69, true, -1); //Pared interna de lab
						
						CollisionX(-89, -54, 68, 69, true, -1); //Pared interna de lab
						CollisionX(-89, 64, -86, -85, true, 1); //Pared interna de lab

						CollisionX(-34.8, 67, 72, 73, true, 1); //Pared EXTERNA de lab
						CollisionX(-94.6, -51, 72, 73, true, 1); //Pared EXTERNA de lab

						CollisionX(-50, -31, 52, 53, true, 1); //Pared cerca de salida de laberinto
						CollisionX(-54, -31, 47, 48, true, -1); //Pared cerca de salida

						CollisionX(-34.9, -15, 33, 34, true, 1);
						CollisionX(-70, -35, 13, 14, true, 1);

						CollisionX(-69, -50, 28, 29, true, -1);
						CollisionX(-69, -50, 33, 34, true, 1);

						CollisionX(-74, -31, 8, 9, true, -1);
						CollisionX(-31, -11, 27, 28, true, -1);

						CollisionX(-121, -94.7, 73, 74, true, 1); //Rocas cerca del laberinto

					     

						//Colisiones en Z
						CollisionZ(-25, 69, -90, -89, true, 1); //Pared interna lab
						CollisionZ(-86, -49, -90, -89, true, 1); //Pared interna lab
						CollisionZ(-86, 67, 63, 64, true, -1); //Pared interna lab

						CollisionZ(53, 72, -50, -49, true, 1); //Pared cerca de salida
						CollisionZ(67, 72, -36, -35, true, -1); //Pared cerca de salida
						CollisionZ(49, 53, -30, -29, true, 1); //Pared cerca de salida
						CollisionZ(48, 68, -56, -55, true, -1); //Pared cerca de salida
						CollisionZ(13, 32.7, -36, -35, true, -1);

						CollisionZ(32, 68.5, -16, -15, true, -1);

						CollisionZ(12, 52.8, -70, -69, true, 1);
						CollisionZ(8.9, 52.8, -76, -75, true, -1);

						CollisionZ(9.41, 28.3, -30, -29, true, 1);
						CollisionZ(29.6, 68, -10, -9, true, 1);

	                        	           // CollisionX(-34, 68, 73, true, 1); //pared externa¿
	                        			   // CollisionZ(69, 73, -36, true, -1); //Donde me muevo en X
	                        
	                        	//			CollisionX(-94, -54, 74, true, 1); //Pared externa de lab
	                        	//	CollisionX(-89, -52, 68, true, -1); //interna


	                        		//CollisionZ(49, 72, -56, true, -1);
	                        		 //CollisionZ(54, 71, -48, true, 1);
                                     #pragma endregion
	                        		 //--------------------------------------------------------------------
	                        
	                        		 //Colisiones de las otras paredes, no todas tienen unu
	                        			//CollisionZ(-86, 66, 64, true, -1);
	                        			//CollisionZ(-86, 66, 64, true, 1);
	                        				//CollisionX(-93, 68, -92, true, -1); //este sí jala XD
	                        				//CollisionX(-92, 66, -83, true, 1); //Este no, son la misma pared
	 swapChain->Present(1, 0);

	}

	void CollisionX(float P1, float P2, float P3, float P4 , bool colision, int move)
	{
		if (colision)
		{
			if (camara->posCam.x > P1 && camara->posCam.x < P2 && camara->posCam.z > P3 && camara->posCam.z < P4)
				camara->posCam.z = camara->posCam.z + move; //Empuja en Z
		}

	}

	//Hace colisión en el eje Z y te empuja en X
	void CollisionZ(float P1, float P2, float P3, float P4, bool colision, int move)
	{
		if (colision)
		{
			if (camara->posCam.z > P1 && camara->posCam.z < P2 && camara->posCam.x > P3 && camara->posCam.x < P4)
				camara->posCam.x = camara->posCam.x + move; // empuja en X
		}

	}


	bool isPointInsideSphere(float* point, float* sphere) {
		bool collition = false;

		float distance = sqrt((point[0] - sphere[0]) * (point[0] - sphere[0]) +
			(point[1] - sphere[1]) * (point[1] - sphere[1]));

		if (distance < sphere[2])
			collition = true;
		return collition;
	}





	//Activa el alpha blend para dibujar con transparencias
	void TurnOnAlphaBlending()
	{
		float blendFactor[4];
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;
		HRESULT result;

		D3D11_BLEND_DESC descABSD;
		ZeroMemory(&descABSD, sizeof(D3D11_BLEND_DESC));
		descABSD.RenderTarget[0].BlendEnable = TRUE;
		descABSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descABSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descABSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descABSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descABSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		result = d3dDevice->CreateBlendState(&descABSD, &alphaBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
	}

	//Regresa al blend normal(solido)
	void TurnOffAlphaBlending()
	{
		D3D11_BLEND_DESC descCBSD;
		ZeroMemory(&descCBSD, sizeof(D3D11_BLEND_DESC));
		descCBSD.RenderTarget[0].BlendEnable = FALSE;
		descCBSD.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		descCBSD.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		descCBSD.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		descCBSD.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		descCBSD.RenderTarget[0].RenderTargetWriteMask = 0x0f;
		HRESULT result;

		result = d3dDevice->CreateBlendState(&descCBSD, &commonBlendState);
		if(FAILED(result))
		{
			MessageBox(0, L"Error", L"Error al crear el blend state", MB_OK);
			return;
		}

		d3dContext->OMSetBlendState(commonBlendState, NULL, 0xffffffff);
	}

	void TurnOnDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDSD;
		ZeroMemory(&descDSD, sizeof(descDSD));
		descDSD.DepthEnable = true;
		descDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDSD.StencilEnable=true;
		descDSD.StencilReadMask = 0xFF;
		descDSD.StencilWriteMask = 0xFF;
		descDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDSD, &depthStencilState);
		
		d3dContext->OMSetDepthStencilState(depthStencilState, 1);
	}

	void TurnOffDepth()
	{
		D3D11_DEPTH_STENCIL_DESC descDDSD;
		ZeroMemory(&descDDSD, sizeof(descDDSD));
		descDDSD.DepthEnable = false;
		descDDSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDDSD.DepthFunc = D3D11_COMPARISON_LESS;
		descDDSD.StencilEnable=true;
		descDDSD.StencilReadMask = 0xFF;
		descDDSD.StencilWriteMask = 0xFF;
		descDDSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		descDDSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		descDDSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		descDDSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		descDDSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		d3dDevice->CreateDepthStencilState(&descDDSD, &depthStencilDisabledState);
		d3dContext->OMSetDepthStencilState(depthStencilDisabledState, 1);
	}

	void billCargaFuego()
	{
		uv1[0].u = .125;
		uv2[0].u = .125;
		uv3[0].u = 0;
		uv4[0].u = 0;

		uv1[0].v = .25;
		uv2[0].v = 0;
		uv3[0].v = 0;
		uv4[0].v = .25;


		for (int j = 0; j < 8; j++) {
			uv1[j].u = uv1[0].u + (j * .125);
			uv2[j].u = uv2[0].u + (j * .125);
			uv3[j].u = uv3[0].u + (j * .125);
			uv4[j].u = uv4[0].u + (j * .125);

			uv1[j].v = .25;
			uv2[j].v = 0;
			uv3[j].v = 0;
			uv4[j].v = .25;
		}
		for (int j = 0; j < 8; j++) {
			uv1[j + 8].u = uv1[0].u + (j * .125);
			uv2[j + 8].u = uv2[0].u + (j * .125);
			uv3[j + 8].u = uv3[0].u + (j * .125);
			uv4[j + 8].u = uv4[0].u + (j * .125);

			uv1[j + 8].v = .5;
			uv2[j + 8].v = .25;
			uv3[j + 8].v = .25;
			uv4[j + 8].v = .5;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 16].u = uv1[0].u + (j * .125);
			uv2[j + 16].u = uv2[0].u + (j * .125);
			uv3[j + 16].u = uv3[0].u + (j * .125);
			uv4[j + 16].u = uv4[0].u + (j * .125);

			uv1[j + 16].v = .75;
			uv2[j + 16].v = .5;
			uv3[j + 16].v = .5;
			uv4[j + 16].v = .75;
		}

		for (int j = 0; j < 8; j++) {
			uv1[j + 24].u = uv1[0].u + (j * .125);
			uv2[j + 24].u = uv2[0].u + (j * .125);
			uv3[j + 24].u = uv3[0].u + (j * .125);
			uv4[j + 24].u = uv4[0].u + (j * .125);

			uv1[j + 24].v = 1;
			uv2[j + 24].v = .75;
			uv3[j + 24].v = .75;
			uv4[j + 24].v = 1;
		}


		
	}

	
	

};
#endif