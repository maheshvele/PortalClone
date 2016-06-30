class RenderingManagerC
{
public:
	static RenderingManagerC	*CreateInstance();
	static RenderingManagerC	*GetInstance() { return sInstance; };
	~RenderingManagerC(){};
	void		init(D3DPRESENT_PARAMETERS m_d3dpp);
	void		update();
	void		shutdown();

	bool PlayPortalSound;

private:
	static RenderingManagerC *sInstance;
	RenderingManagerC(){};

	LPDIRECT3D8             g_pD3D      ; // Used to create the D3DDevice
	BYTE*                   g_pResourceSysMemData ; // Sysmem data for the packed resource
	BYTE*                   g_pResourceVidMemData ; // Vidmem data for the packed resource


	LPDIRECT3DVERTEXBUFFER8 g_pVB				  ;          // Buffer to hold vertices
	LPDIRECT3DTEXTURE8      g_backGroundTexture   ; // Our texture
	LPDIRECT3DTEXTURE8      g_playerTexture       ;
	LPDIRECT3DTEXTURE8      g_crossHairTexture    ;
	LPDIRECT3DTEXTURE8		g_playerSpriteSheetTexture;
	LPDIRECT3DTEXTURE8      g_playerHandTexture;
	LPDIRECT3DTEXTURE8		g_turretTexture;

	LPD3DXSPRITE backGround;
	LPD3DXSPRITE player;
	LPD3DXSPRITE crossHair;
	LPD3DXSPRITE playerHand;
	LPD3DXSPRITE turret;

	RECT CurrentPortal;

	LPD3DXSPRITE playerSpriteSheet;

	HRESULT LoadPackedResource();
	HRESULT LoadPackedResource2();

	
	void DrawBackground();
	void DrawRaycast();
	void DrawPlayer();
	void DrawPortal(bool IsOrangePortal);
	void DrawTurrents();
	void DrawCrossHair();
	void DrawStaticObjects();
	void DrawBox( FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2,
	DWORD dwFillColor, DWORD dwOutlineColor);

	

};

#define PORTAL_DRAW_HEIGHT 29f
#define PORTAL_DRAW_WIDTH 59f
