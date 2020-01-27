#ifndef VGUI_H
#define VGUI_H

#include "../../../StdAfx.h"
#include "../../Tools/rtools.h"
#include "../vtexture.h"
#include "../vengine.h"

#define AttachFunctionPointer(a,b) *(long*)(&(a)) = (long)((void*)(*(long*)((long)&b)));

class ObjectHandle;
class GImageList;

class VFont // TODO: improve texture font system
{
public:
	int fbase;
	long ftex;
	int fsize;
	int* cwid; // char widths

	VFont();
	~VFont();
	void PrintText(long x,long y,const char* fmt, ...);	
	void LoadFromSystem(char *fontname, int size);
	void LoadFromTexture(VTexture* tex,word wp,word hp,word start,word len);
	long GetStringLen(char* strs,long ln = 0);
	void UnloadFont();
};

#define GOBJECTHANDLE_CLASSID		0
#define GWINDOW_CLASSID				1
#define GBUTTON_CLASSID				2
#define GIMAGE_CLASSID				3
#define GCHECKBOX_CLASSID			4
#define GVSCROLLBAR_CLASSID			5
#define GLISTBOX_CLASSID			6
#define GCOMBOBOX_CLASSID			7
#define GTEXTBOX_CLASSID			8
#define GPROGRESSBAR_CLASSID		9
#define GMENU_CLASSID				10
#define GMENUITEM_CLASSID			11
#define GMENUBAR_CLASSID			12
#define GMENUBARITEM_CLASSID		13
#define GDESKTOP_CLASSID			14
#define GTOOLBAR_CLASSID			15
#define GTOOLBARBUTTON_CLASSID		16
#define GSPLITCONTAINER_CLASSID		17
#define GTREEVIEW_CLASSID			18
#define GTREENODE_CLASSID			19
#define GGROUPBOX_CLASSID			20
#define GLISTVIEW_CLASSID			21
#define GLISTITEM_CLASSID			22
#define GCOLUMNHEADER_CLASSID		23

#define GSPLIT_VERTICAL				0
#define GSPLIT_HORIZONTAL			1

#define GDATATYPE_CHARPTR			0
#define GDATATYPE_INT				1
#define GDATATYPE_FLOAT				2
#define GDATATYPE_OBJECTPTR			3

#define GTEXTALIGN_LEFT				0
#define GTEXTALIGN_CENTER			1
#define GTEXTALIGN_RIGHT			2

enum gdatatypes
{
	type_charptr,
	type_int,
	type_float,
	type_objectptr,
};


#define GLISTVIEW_ICON				0
#define GLISTVIEW_SMALLICON			1
#define GLISTVIEW_LIST				2
#define GLISTVIEW_REPORT			3

#define GSTYLE_UPDATING		0x00000001
#define GSTYLE_VISIBLE		0x00000002
#define GSTYLE_WAITING		0x00000004

#define GEVENT_MOUSEDOWN	0x00000001
#define GEVENT_MOUSEUP		0x00000002
#define GEVENT_KEYDOWN		0x00000004
#define GEVENT_KEYUP		0x00000008
#define GEVENT_MOUSEWHEEL	0x00000010
#define GEVENT_KEYPRESS		0x00000020

#define GDOCK_NODOCK		0
#define GDOCK_TOP			1
#define GDOCK_LEFT			2
#define GDOCK_RIGHT			3
#define GDOCK_BOTTOM		4
#define GDOCK_FILL			5

#ifndef WHEEL_DELTA
#define WHEEL_DELTA			120
#endif

struct gevent 
{
	int mevent;
	int x;
	int y;
	char keycode;
	bool inside;
};

class GuiSkin;
class ObjectContainer;

class ObjectHandle // can be usable for container area
{
public:
	ObjectHandle();
	ObjectHandle*	pid;		// parent id
	
	long			classID;	// it will identify master class

	int				x;			// x
	int				y;			// y
	int				w;			// width
	int				h;			// height
	int				sx;			// screen x
	int				sy;			// screen y
	int				zorder;		// zorder

	long			style;		
	long			extstyle;

	GuiSkin*		cskin;

	bool			IsMouseIn;
	bool			updating;
	bool			visible;
	bool			container;
	bool			carescissor;

	void			SetRelative();

	void			RenderDebugArea();

	virtual void	render					()										{};
	virtual void	doevents				()										{ if(!visible) return; DeliverEvents(); };
	virtual void	update					()										{ if(!visible) return; SetRelative(); };
	virtual void	resize					()										{}; // it has nothing to do when not instanced

	void			DeliverEvents			();

	bool			IsMouseInside();
	
	// Event CallBacks
	void			*pClass;				// if event callbacks is in a class then set this
	void			(*MouseDown0)			(int,int,int);
	void			(*MouseUp0)				(int,int,int,bool);
	void			(*MouseMove0)			(int,int,int);
	void			(*MouseOut0)			();
	void			(*MouseWheel0)			(int,int,int);	
	void			(*KeyDown0)				(int);
	void			(*KeyUp0)				(int);
	void			(*GotFocus0)			();
	void			(*LostFocus0)			();
	void			(*KeyPress0)			(int);	// used when character capture
 
	int				pdock;
	void			Dock(int pos,ObjectContainer* cont);

	void			SetFocus();

	virtual void	LoadHandle(int in_x,int in_y,int in_w,int in_h,long in_style,GuiSkin* in_skin = 0);

	ObjectHandle*	nextobject;
	ObjectHandle*	prevobject;
};

class ObjectContainer: public ObjectHandle
{
public:
	ObjectContainer();
	ObjectHandle* firstobject;
	ObjectHandle* lastobject;	// ayni zamandada aktif obje
	
	long objectcount;

	// client area
	int padtop;
	int padleft;
	int padbottom;	
	int padright;

	bool defcarescissor;

	void AddObj(ObjectHandle* bs);
	void DelObj(ObjectHandle* bs);

	ObjectHandle* owner;

	void ContainerSendEvents();	// send events to all objects in this container
	void ContainerRender();		// render all objects in this container
	void ContainerUpdate();

	void ActivateObject(ObjectHandle* bs); // makes a object to last object

	void DrawHitboxes();
	void SetUpdating(bool val);
};

class GuiScalableQuad
{
public:
	float sc_uv[8];
	long sc_h[4];
	void GenerateUV(VTexture *tx,long x1,long y1,long x2,long y2);
	void Render(ObjectHandle *dp);
	void Render(long x,long y,long w,long h);
};

class GuiVerticalQuad
{
public:
	float sc_uv[6];
	long sc_h[2]; // 0 = top margin, 1 = bottom margin
	void GenerateUV(VTexture *tx,long x1,long y1,long x2,long y2);
	void Render(ObjectHandle *dp);
	void Render(long x,long y,long w,long h);
};

class GuiHorizontalQuad
{
public:
	float sc_uv[6];
	long sc_h[2]; // 0 = top margin, 1 = bottom margin
	void GenerateUV(VTexture *tx,long x1,long y1,long x2,long y2);
	void Render(ObjectHandle *dp);
	void Render(long x,long y,long w,long h);
};

class GuiQuad
{
public:
	float sc_uv[4];
/*	int w;
	int h;*/
	void GenerateUV(VTexture *tx,long x1,long y1,long x2,long y2);
	void Render(ObjectHandle *dp);
//	void Render(long x,long y,bool ld_idty = true);
	void Render(long x,long y,long w,long h,bool ld_idty = true);
};

class GuiSkin // holds data for fast rendering
{
public:
	GuiSkin();

	VFont* basefont;
	byte deffontclr[3];

	long skinID;
	VTexture basetex;
	VPackNode basequad;
	void BeginCreatingSkin(int w,int h);
	void EndCreatingSkin();
	
	long window_tex[2];
	bool window_cac; // window caption's center aligning
	float window_c[4]; // window color with alpha .)
	GuiScalableQuad window_quad;
	void LoadWindow(VTexture *tex,long mleft,long mright,long mtop,long mbottom);
	void CreateWindowTexture(VTexture *tex,bool active,int w,int h,char* bbpath,char* blpath,char* brpath,char* btpath,int tl,int tr,int lt,int lb,int rt,int rb,int bl,int br,long bgclr); // Texture pointer, buffer width,buffer height, border bottom path, border left path ...., TL=top left margin, TR= Top right margin .... rt = right top margin ...
	void CreateLoadWindowTexture(VTexture *tex,int w,int h,char* bbpath,char* blpath,char* brpath,char* btpath,int tl,int tr,int lt,int lb,int rt,int rb,int bl,int br,long bgclr); // Texture pointer, buffer width,buffer height, border bottom path, border left path ...., TL=top left margin, TR= Top right margin .... rt = right top margin ...
	
	int win_closew;
	int win_closeh;
	GuiQuad win_close[6];
	void LoadWindowCloseButton(VTexture* but);

	GuiScalableQuad button_quad[5];
	void LoadButton ( VTexture *tex,long mleft,long mright,long mtop,long mbottom); // TEXTURE AND MARGINS

	dword cbox_base;
	long cbox_w,cbox_h;
	void LoadCheckBox( VTexture *tex);

	dword vsb_base; // gl list base for buttons
	long vsb_w,vsb_h;
	GuiQuad vss_quad[4]; // for 4 state
	GuiVerticalQuad vst_quad[3];

	GuiQuad hss_quad[4]; // horizontal shaft
	GuiHorizontalQuad hst_quad[3]; // horizontal thumb

	void LoadScrollButton	( VTexture *but );
	void LoadVScrollShaft	( VTexture *shaft );
	void LoadVScrollThumb	( VTexture *thm,long mtop,long mbottom);
	void LoadHScrollShaft	( VTexture *shaft );
	void LoadHScrollThumb	( VTexture *thm,long mleft,long mright);
	void CreateVScrollButton ( VTexture *but, VTexture *gly); // that works COOL!

	GuiScalableQuad sunkedge_quad[2]; // second for disabled
	void LoadSunkEdge(VTexture *tex,long mleft,long mright,long mtop,long mbottom);

	long cbb_w,cbb_h;	// combo box button width and height
	GuiQuad cbb_quad[4];

	void LoadComboBoxButton ( VTexture *but );
	void CreateComboBoxButton ( VTexture *but, VTexture *gly);

	long tbeam_tex;		// text beam texture
	void LoadTextBeam ( VTexture *beam );

	long pchk_wid;		// progress bar chunk width
	bool presizing;		// progress bar chunk is resizing
	GuiScalableQuad pbar_quad;
	GuiScalableQuad pchk_quad;

	void LoadProgressBar(VTexture *but,long mleft,long mright,long mtop,long mbottom);
	void LoadProgressChunk(VTexture *but);
	void LoadProgressResizingChunk(VTexture *but,long mleft,long mright,long mtop,long mbottom);

	GuiScalableQuad mnb_quad;
	GuiScalableQuad mni_quad[5]; // menu item quads // normal, seprator, disabled, mouse over,default
	GuiQuad mis_quad; // menu item_seprator quad, use this for seprator
	GuiScalableQuad mbi_quad[2]; // menubar item quads // normal, pressed
	
	void LoadMenuBackground(VTexture *but,long mleft,long mright,long mtop,long mbottom);
	void LoadMenuItem(VTexture *but,long mleft,long mright,long mtop,long mbottom,long sep_top,long sep_bottom); // s
	void LoadMenuBarItem(VTexture *but,long mleft,long mright,long mtop,long mbottom);
	

	GuiScalableQuad tbr_quad;		// toolbar quad
	GuiScalableQuad tbb_quad[5];	// toolbar button quad
	GuiVerticalQuad rbg_quad;		// rebar grip quad
	
	void LoadToolbarBackground(VTexture *but,long mleft,long mright,long mtop,long mbottom);
	void LoadToolbarButton(VTexture *but,long mleft,long mright,long mtop,long mbottom);
	void LoadRebarGrip (VTexture *but,long mtop,long mbottom);

	int tv_w; // treeview expander width
	int tv_h; // treeview expander height
	int tv_oy; // treeview offset y
	GuiQuad tv_quad[2];	// treeview quad
	void LoadTreeViewExpander(VTexture *but);

	GuiScalableQuad gbx_quad[2]; // group box quad
	void LoadGroupBox ( VTexture *but, long mleft, long mright, long mtop, long mbottom);

	int lv_imgoffsetx;
	int lv_imgoffsety;
	int lv_colh_h; // column header height
	GuiHorizontalQuad lv_colheader[5];
	void LoadColumnHeader ( VTexture *but,long mleft,long mright);

	int trbar_h; // trackbar height
	int trbar_w; // trackbar width
	int h_sthm_w; // slider thumb width ( for horizontal slider)
	int h_sthm_h;
	int v_sthm_w;
	int v_sthm_h; // slider thumb height ( for vertical slider )
	//GuiQuad htrbar; // horizontal trackbar
	GuiHorizontalQuad htrbar;
	GuiQuad vtrbar; // vertical trackbar
	GuiQuad hsthm[3][5]; // horizontal slider's thumb, 3 style, and 5 state for every style
	GuiQuad vsthm[3][5]; // horizontal slider's thumb, 3 style, and 5 state for every style
	void LoadHSliderTrackbar( VTexture *tex ,int mleft,int mright);
	void LoadVSliderTrackbar( VTexture *tex );
	void LoadHSliderThumb ( VTexture* tex, int style );
	void LoadVSliderThumb ( VTexture* tex, int style );


	void MakeDefault();

	void SaveGUI(char* rgipath); // reaction gui
	void LoadGUI(char* rgipath);
};                                                         

extern GuiSkin* DefaultSkin;

class GWindow: public ObjectContainer
{
private:
	int cx,cy;
	int rx,ry;
	int slen;
	int swid;// captions length by pixel
	int stx,sty;
	int dtx,dty;
	bool moving;
	bool IsGrab();

	ObjectHandle cbutton; // closebutton
	void cbutton_MouseDown		(int x,int y,int button);
	void cbutton_MouseUp		(int x,int y,int button,bool inside);
	void cbutton_MouseMove		(int x,int y,int button);
	void cbutton_MouseOut		();
	int cbstate; // close button state


public:
	GWindow();

	// properties
	char*			caption;
	void			setcaption(char* st,...);
	void			refresh();
	float			caption_fc[4]; // caption fore color
	//ObjectContainer container;
	int				inactive;

	void MouseDown(int x,int y,int button);
	void MouseUp(int x,int y,int button,bool inside);

	void LoadHandle(int in_x,int in_y,int in_w,int in_h,long in_style,GuiSkin* in_skin = 0);

	// events
	/*void (*onMouseMove)(int,int);
	void (*onMouseDown)(int,int,char); // x,y,button
	void (*onMouseUp)(int,int,char);
	void (*onKeyDown)(char);
	void (*onKeyUp)(char);*/

	//render
	void			doevents();
	void			render();
};

class GHSlider: public ObjectHandle
{
private:
	ObjectHandle bthm;
	void bthm_MouseDown		(int x,int y,int button);
	void bthm_MouseUp		(int x,int y,int button,bool inside);
	void bthm_MouseMove		(int x,int y,int button);
	void bthm_MouseOut		();
	int s_bthm; // close button state

	bool moving;
	int dtx;

	int py;
	float freq;
	float stotal;

public:
	GHSlider();

	int style;
	float minv;
	float maxv;
	float value;

	bool fixedvalue;

	void valuechanged();
	void refresh();
	void initialize();

	void doevents();
	void update();
	void render();
};

class GImage: public ObjectHandle
{
public:
	GImage();

	void			MouseDown		(int x,int y,int button);

	void			(*onMouseDown)	(int,int,int);

	dword			texid;
	VTexture		*mtex;
	
	void			PrepareImage(VTexture* vtex);
	void			PrepareImage(VTexture* vtex,int msx,int msy,int mw,int mh);


	bool			customdraw;
	float			tuv[4];

	void			render();
};

class GButton: public ObjectHandle
{
private:
	int slen; // caption length
	int cx,cy; // caption position
	int rx,ry; // captions relative poistion
	int swid; // captions length by pixel
	long tcounter;
	
public:
	GButton();

	// properties
	char*			caption;
	int				state;
	bool			enabled;
	bool			continuous;
	long			delay;
	void			setcaption(char* st,...);
	float			caption_fc[4]; // caption fore color

	void MouseDown(int x,int y,int button);
	void MouseUp(int x,int y,int button,bool inside);
	void MouseMove(int x,int y,int button);	
	void MouseOut();
	
	// events
	void (*onClick)();

	// render
	void			doevents();
	void			render();
	void			update();
};

class GCheckBox: public ObjectHandle
{
private:
	int slen; // caption length
	int cx,cy; // caption position
	int rx,ry; // captions relative poistion
	int swid; // captions length by pixel
	bool dn;

public:
	GCheckBox();

	// properties
	char*			caption;
	int				state;
	int				value; // actual state
	bool			enabled;
	void			setcaption(char *st,...);
	void			SetValue(int vl);

	float			caption_fc[4]; // caption fore color

	void			MouseUp(int x,int y,int button,bool inside);
	void			MouseMove(int x,int y,int button);
	void			MouseOut();

	// render
	void			doevents();
	void			render();
	void			update();
};

class GVScrollBar: public ObjectHandle
{
private:
	void bup_MouseDown		(int x,int y,int button);
	void bup_MouseUp		(int x,int y,int button,bool inside);
	void bup_MouseMove		(int x,int y,int button);
	void bup_MouseOut		();

	void bdwn_MouseDown		(int x,int y,int button);
	void bdwn_MouseUp		(int x,int y,int button,bool inside);
	void bdwn_MouseMove		(int x,int y,int button);
	void bdwn_MouseOut		();

	void bthm_MouseDown		(int x,int y,int button);
	void bthm_MouseUp		(int x,int y,int button,bool inside);
	void bthm_MouseMove		(int x,int y,int button);
	void bthm_MouseOut		();

	int				thumbh;
	int				shafth;
	int				exh;
	int				extradatah;
	float			freq; // value to shaft height ratio
	int				max;
	int				dtx,dty;
	bool			moving;
	long			fdown; // first down tick
	bool			firststage;
	bool			secondstage;
	bool			updown; // up or down? up=true
	
public:
	GVScrollBar();
	
	ObjectHandle	bup,bdwn,bthm;

	void			refresh();
	void			fixedrefresh();
	void			initialize();
	void			valuechanged();
	
	// properties
	int				svisible;
	int				stotal;
	int				smallchange;
	int				largechange;
	int				value;
	bool			align; // true for vertical, false for horizontal

	int				s_ubut; // up button's state
	int				s_dbut; // down button's state
	int				s_shft; // shaft's state
	int				s_tbut; // thumb button's state

	// render
	void			doevents();
	void			render();
	void			update();
};

class GHScrollBar: public ObjectHandle
{
private:
	void bup_MouseDown		(int x,int y,int button);
	void bup_MouseUp		(int x,int y,int button,bool inside);
	void bup_MouseMove		(int x,int y,int button);
	void bup_MouseOut		();

	void bdwn_MouseDown		(int x,int y,int button);
	void bdwn_MouseUp		(int x,int y,int button,bool inside);
	void bdwn_MouseMove		(int x,int y,int button);
	void bdwn_MouseOut		();

	void bthm_MouseDown		(int x,int y,int button);
	void bthm_MouseUp		(int x,int y,int button,bool inside);
	void bthm_MouseMove		(int x,int y,int button);
	void bthm_MouseOut		();

	int				thumbh;
	int				shafth;
	int				exh;
	int				extradatah;
	float			freq; // value to shaft height ratio
	int				max;
	int				dtx,dty;
	bool			moving;
	long			fdown; // first down tick
	bool			firststage;
	bool			secondstage;
	bool			updown; // up or down? up=true
	
public:
	GHScrollBar();
	
	ObjectHandle	bup,bdwn,bthm;

	void			refresh();
	void			fixedrefresh();
	void			initialize();
	void			valuechanged();
	
	// properties
	int				svisible;
	int				stotal;
	int				smallchange;
	int				largechange;
	int				value;
	bool			align; // true for vertical, false for horizontal

	int				s_ubut; // up button's state
	int				s_dbut; // down button's state
	int				s_shft; // shaft's state
	int				s_tbut; // thumb button's state

	// render
	void			doevents();
	void			render();
	void			update();
};

class GScrollbarArea: public ObjectHandle
{
private:
	int cornerx;
	int cornery;
	bool a,b;
	int state; // 0 = no scrbar, 1 = vert, 2 = horz, 3 both

	void PrepareOnlyVertical();
	void PrepareOnlyHorizontal();
	void PrepareBoth();
	void DisableBoth();

public:
	GScrollbarArea();

	GHScrollBar hscr;
	GVScrollBar vscr;
	
	bool vscr_enabled;
	bool hscr_enabled;

	void*	pEventClass;
	void	(*onStateChanged)(int); // returns changed state


	void RefreshScrollbars();
	void SwitchState(int newst);
	//void Resize(bool vert,bool horz);
//	void Resize();

	void update();
	void doevents();
	void render();
};

class GListBox: public ObjectHandle
{
private:
	void			Precache();
	void			MouseUp(int x,int y,int button,bool inside);
	void			MouseWheel(int x,int y, int delta);
	void			LostFocus();

public:
	GListBox();
	int				state;
	int				viewmax;
	void*			pEventClass;

	GVScrollBar		scr;

	int				ListIndex;
	char**			ListData;
	long			ListCount;
	void			AddItem(char *st,...);
	void			RemoveItem(int id);
	char*			ListItem(int id);
	void			Clear();
	float			list_fc[4]; // list fore color
	

	void			refresh();

	void			(*onSelectionChanged)(int); // returns id of changed item
	void			(*onLostFocus)();

	// render
	void			doevents();
	void			render();
	void			update();
};

class GComboBox: public ObjectHandle
{
private:
	void ddbut_MouseDown		(int x,int y,int button);
	void ddbut_MouseUp			(int x,int y,int button,bool inside);
	void ddbut_MouseMove		(int x,int y,int button);
	void ddbut_MouseOut			();
	void lst_onSelectionChanged	(int indx);
	void lst_onLostFocus		();
	void MouseUp				(int x,int y,int button,bool inside);

public:
	GComboBox();
	ObjectHandle	ddbut; // drop down button
	GListBox		lst;
	

	void			initialize();

	int				state;
	int				s_udbut;
	char*			text;
	float			cbox_fc[4]; // list fore color

	// render
	void			doevents();
	void			render();
	void			update();
};

class GTextBox: public ObjectHandle
{
private:
	int cursorx;
	int textx;
	int lastkey;
	int memsize;
	int tcounter;
	bool wfkeyup;	// wait for keyup?

public:
	GTextBox();

	char* text;

	void KeyPress	(int key);
	void KeyDown	(int key);
	void KeyUp		(int key);

	// render
	//void		doevents();
	void		render();
	//void		update();
};

class GProgressBar: public ObjectHandle
{
private:
	char cperc[8]; // percentage in char
	int cperclen; // percentage length in pixels
public:
	GProgressBar();

	float percent;
	bool showpercent;
	bool smooth;

	void render();
};

class GMenuItem;

class GMenu: public ObjectContainer
{
private:
	int maxwidth;
	int lasty;
	void FixWidths();
	
public:
	GMenu();

	void LostFocus();

	//ObjectContainer items;

	void AddItem(char* caption,int imgid,void* func,GImageList* imglst=0);
	//void AddItem(GMenuItem* mnitem);
	void AddSeprator();

	void doevents();
	void render();
	void update();
};

class GMenuItem: public ObjectHandle
{
public:
	GMenuItem();

	void MouseMove		(int x,int y,int button);
	void MouseOut		();
	void MouseUp		(int x,int y,int button,bool inside);
	
	GImageList* imglst;
	GMenu* parent;
	char* caption;
	int imgid;
	bool seprator;
	
	int state;

	void (*onClick)();
	
	void render();
	//void doevents();
};

class GMenuBarItem;

class GMenuBar: public ObjectContainer
{
private:
	int lastx;

public:
	GMenuBar();

	void LostFocus();

	//ObjectContainer items;
	bool state;

	void SetupToWindow(GWindow* tw);
	void CloseAllMenus();

	GMenu* currentmenu; // current visible menu

	GMenuBarItem* AddItem(char* caption,int imgid,void* func);
	GMenuBarItem* AddItem(char* caption,int imgid,GMenu* submenu);
	//void AddItem(GMenuItem* mnitem);

	void doevents();
	void render();
	void update();
};

class GMenuBarItem: public ObjectHandle
{
public:
	GMenuBarItem();

	void MouseDown		(int x,int y,int button);
	void MouseUp		(int x,int y,int button,bool inside);
	void MouseMove		(int x,int y,int button);
	void MouseOut		();

	GMenuBar* parent;
	char* caption;
	int imgid;
	//bool seprator;
	GMenu* submenu;
	
	int state;

	void (*onClick)();
	
	
	//void doevents();
	void render();
	//void update();
};

class GImageList
{
private:
	VPackNode basequad;

public:
	GImageList();

	VTexture image;
	long imagelistID; // opengl texture id

	int* imagew;
	int* imageh;
	GuiQuad** imagequads;
	long imagecount;

	void Precache();
	void CreateBuffer(int w,int h);
	int AddImage(VTexture* tx); // returns image id

	void render(int x,int y,int imgid,bool ld_idty = true); // render normal size
	//void render(int x,int y,int w,int h,int imgid); // render custom size*/

	void LoadImageList(char* fpath);
	void SaveImageList(char* fpath);
};

class GDesktop: public ObjectContainer
{
public:
	GDesktop();

	//ObjectContainer container;

	void CheckWindowActivation(); // it will set the activated window
	void DrawGui();
	void CustomDrawGui();
};

extern GDesktop mDesktop;

class GPanel: public ObjectContainer
{
public:
	GPanel();

	void doevents();
	void render();
	void update();
};

class GGroupBox: public ObjectContainer
{
public:
	GGroupBox();

	char* caption;

	void doevents();
	void render();
	void update();
};

class GToolbarButton;

class GToolbar: public ObjectContainer
{
private:
	int lastx;
	int maxw;
	int maxh;

public:
	GToolbar();

	void SetupToWindow(GWindow* tw,int dock);
	GToolbarButton* AddItem(char* caption,void* func,int bstyle,int imgid,GImageList* imglst);

	void doevents();
	void render();
	void update();
};

class GToolbarButton: public ObjectHandle
{
public:
	GToolbarButton();

	int swid; // captions length by pixel
	int cx,cy; // caption position
	int ix,iy; // image x, image y
	int iw,ih;

	void MouseDown		(int x,int y,int button);
	void MouseUp		(int x,int y,int button,bool inside);
	void MouseMove		(int x,int y,int button);
	void MouseOut		();

	GToolbar* parent;
	GImageList* imglst;
	char* caption;
	int imgid;
	int bstyle;	// 0 = justimage, 1 = image& text, 2 = text 
	
	int state;

	void (*onClick)();

	void render();
};

class GSplitContainer: public ObjectHandle
{
private:
	int stx,sty;
	int dtx,dty;
	bool moving;
	
public:
	GSplitContainer();

	int orientation; // 0= vertical split , 1 = horizontal split

	GPanel panel1;
	GPanel panel2;

	int splitterdistance;
	int splitterwidth;

	bool fixed; // false for dynamic update
	bool border;

	void UpdatePanels();

	void MouseDown(int x,int y,int button);
	void MouseUp(int x,int y,int button, bool inside);

	void SetupPanels(int orient,int splitdist); // starter function

	void doevents();
	void render();
	void update();
};


class GTreeview;
// treeview test basic
class GTreeNode: public ObjectContainer
{
public:
	GTreeNode();

	void MouseDown		(int x,int y,int button);
	void MouseWheel		(int x,int y, int delta);
	//int lasty;
	int starty;
	int reqh;

	char* GetFullPath();

	int level;
	
	GTreeNode* AddItem (char* caption,int imgid,GImageList* imglst);

	int imgid;
	GTreeview* parent;
	GImageList* imglst;
	char* caption;

	bool collapsed;

	//void update();
	void doevents();
	void render();
};

class GTreeview: public ObjectContainer
{
public:
	GTreeview();

	int starty;	// it will be the scrollbar value
	int reqh; // required height (max scrollbar value)

	bool showln; // show line
	bool showpm; // show plus minus
	bool showpic; // show picture

	char pathseprator[8];

	GVScrollBar		scr;

	GTreeNode *snode; // selected node
	GTreeNode basenode; // herseyin ele basi bu lan
	
	void MouseWheel(int x,int y, int delta);

	void (*onSelectionChanged)();

	//void EnsureVisible(x); // bisey gorülür olana kadar alkali nodeleri acar
	void refresh();

	void update();
	void doevents();
	void render();
};

class GListItemData;
class GListItem;
class GListview;

class GColumnHeader: public ObjectHandle
{
private:
	int stx,sty;
	int dtx,dty;
	int dtw; // delta t of column widths
	bool moving;
	bool movingfromleft;

	int strpxlen; // pixel length of caption
	int strln;	// length of caption

	int cx,cy;

public:
	GColumnHeader();

	void MouseDown		(int x,int y,int button);
	void MouseUp		(int x,int y,int button,bool inside);
	void MouseMove		(int x,int y,int button);
	void MouseOut		();

	int state;
	
	int textalignment; // 0 left, 1 = center, 2 = right
	void CalculateTextAlignment();


	GColumnHeader* prv;

	GListview* parent;
	char* caption;
	GImageList* imglst;
	int imgid;
	void SetCaption(char* capi);

	//void update();
	void doevents();
	void render();
};

class GListItemData
{
public:
	GListItemData();

	gdatatypes data_type; // even it can be an objecthandler! it can be a button! it can be a progress bar!
	
	union
	{
		char*	chrptr; // it uses same memory point for 4 diffrent data types
		int		intdata;
		float	floatdata;
		void*	objptr;
	} itemdata;

	char* strrep; // string representation of data

	void SetValue(int data);
	void SetValue(char* data);
	void SetValue(float data);
	void SetValue(void* objptr);
	// todo: add functions for changing data
};

class GListItem: public ObjectHandle
{
public:
	GListItem();

	void MouseDown		(int x,int y,int button);
	void MouseWheel		(int x,int y, int delta);

	int cx;
	int strpw; // string pixel width

	GListview* parent;
	GListItemData caption;
	GImageList* imglst;
	int smlimgid;
	int imgid;
	GListItemData** subdata;

	bool selected;

	void SetCaption(char* cp);

	//void update();
	//void doevents();
	void render();
	//void column_render(int colid); // render for reportview
};

class GListview: public ObjectHandle
{
private:
	int maxhiconcount;
	int maxviconcount;
	int reqh;
	int reqw;
	bool twosbars; // when two scrollbars are visible this is enabled
	void StateChanged(int newst);
	void FixColumnPrevs();

public:
	GListview();

	/*GVScrollBar		vscr;
	GHScrollBar		hscr;*/
	GScrollbarArea scr;

	int r_startitem; // rendering will be start from this item
	int r_enditem; // rendering will be end when reach this item

	void MouseWheel		(int x,int y, int delta);

	bool multiselect;
	bool gridcollines; // column gridlines
	bool gridrowlines; // row gridlines

	int maxstrpw; // maximum string pixel width
	
	int rendermode;		// 0 = icon, 1 = small icon, 2= list , 3 = report
	void ChangeViewMode(int rmode);

	GListItem* sitem; // selecteditem

	GListItem** items; // it must work like an array for optimisation
	int itemcount;
	int itemarraysize;
	GListItem* AddItem(char* caption,int smlimgid,int imgid,GImageList* imglst);
	void PrecacheItems();

	GColumnHeader** columns;
	int columnwidth;
	int columncount;
	int columnarraysize;
	GColumnHeader* AddColumn(char* caption,int width,int textalign,int imgid,GImageList* imglst);
	void PrecacheColumns();
	void UpdateColumnWidth();
	void HideColumn(int cid);	// todo: implement this
	void ShowColumn(int cid);	// todo: implement this

	void refresh();
	void UpdateScrollbar();
	void CalculateIconViewStartEnd();
	void CalculateListViewStartEnd();
	void CalculateReportStartEnd();
	void SetVisiblePortion();

	void update();
	void doevents();
	void render();
};


void BeginGUI(); // prepare engine to gui rendering, push matrixes attributes and other infos
void EndGUI(); // end of gui rendering, pop matrixes attributes and other's
//void LoadHandle(ObjectHandle *hnd,int x,int y,int w,int h,bool vis,ObjectHandle *parent = 0);
void DrawQuad(int x1,int y1,int x2,int y2,float s1,float t1,float s2,float t2);
//bool IsInside(ObjectHandle* obj);
//bool IsMouseInsideScissor();
bool IsInside(int x1,int y1,int x2,int y2);


void EnableEventScissor(int x,int y,int w,int h);
void KeyDownHandler(int keycode);
void KeyUpHandler(int keycode);
void MouseDownHandler(int x,int y,char button);
void MouseUpHandler(int x,int y,char button);
void MouseWheelHandler(int x,int y,int delta);
void KeyPressHandler(int keycode);

void SetFocused(ObjectHandle *foc);
long GetFocused();

void SetDebugBox(GListBox* abow);

void GUIOptimize(VEngine *e);

#endif