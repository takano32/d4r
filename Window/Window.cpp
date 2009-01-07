#pragma comment (lib, "msvcrt-ruby18.lib")
#include <ruby.h>

#include "Window.h"
#include "WindowClass.h"
using namespace base;

static LRESULT CALLBACK window_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

struct WindowData {
	WindowClass* klass;
	VALUE loop_action;
};

static void window_dfree( struct WindowData* data ) {
	if( data->klass ) {
		delete data->klass;
		data->klass = NULL;
	}
}

static VALUE window_alloc(VALUE klass)
{
    struct WindowData *data = ALLOC(struct WindowData);
    return Data_Wrap_Struct(klass, 0, window_dfree , data);
}


static VALUE window_initialize( VALUE self) {
    struct WindowData *data;
    Data_Get_Struct(self, struct WindowData, data);
	WindowClass* klass = data->klass = new WindowClass();
	klass->SetWindowProcedure( window_WndProc );
	klass->SetUserData( self );
    return self;
}

static VALUE window_create( VALUE self ) {
	struct WindowData *data;
    Data_Get_Struct(self, struct WindowData, data);
	data->klass->Create();
	return self;
}

static VALUE window_show( VALUE self ) {
	struct WindowData *data;
    Data_Get_Struct(self, struct WindowData, data);
	data->klass->Show();
	return self;
}

static VALUE window_main( VALUE self ) {
	struct WindowData *data;
    Data_Get_Struct(self, struct WindowData, data);
	data->klass->Main();
	return self;
}

static VALUE window_get_handle( VALUE self ) {
    struct WindowData *data;
	Data_Get_Struct(self, struct WindowData, data);
	HWND handle = data->klass->GetWindowHandle();
	if( !handle ) {
		return Qnil;
	}else{
		return INT2FIX(handle);
	}
}


static VALUE window_set_loop_action( VALUE self, VALUE listener ) {
    struct WindowData *data;
	Data_Get_Struct(self, struct WindowData, data);
	data->loop_action = listener;
	data->klass->SetLoopAction( listener );
	//rb_funcall(listener, rb_intern("action"), 0);
	return Qtrue;
}

void Init_window() {
    VALUE Window;

    Window = rb_define_class("Window", rb_cObject);
    rb_define_alloc_func(Window, window_alloc);
    rb_define_private_method(Window, "initialize", (VALUE (*)(...))window_initialize, 0);
	rb_define_method(Window, "create", (VALUE (*)(...))window_create,0);
	rb_define_method(Window, "show", (VALUE (*)(...))window_show,0);
	rb_define_method(Window, "main", (VALUE (*)(...))window_main,0);
	rb_define_method(Window, "handle",(VALUE (*)(...))window_get_handle, 0);
	rb_define_method(Window, "loop_action=",(VALUE (*)(...))window_set_loop_action, 1);
}




LRESULT CALLBACK window_WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam ) {
	VALUE window = (VALUE)WindowClass::GetUserData( hwnd );
	//WindowClass* window = (WindowClass*)GetUserData( hwnd );
	// Windows からのメッセージにより分岐
	// Listenerを導入する予定．
	switch (message){
	case WM_CREATE:
		//window_message( window, rb_str_new2("WM_CREATE") );
		break;
	case WM_KEYDOWN:				// キー入力
        switch( wParam ){			// キーに応じて処理 
		case VK_ESCAPE:				// 終了ボタン
		case VK_F12:
			PostMessage( hwnd, WM_CLOSE, 0, 0 );	// Windowsへ プログラムおわり
			break;
		}
		break;	
		case WM_DESTROY:		// プログラム終了
			PostQuitMessage(0);
			return 0;
			break;
		default:
			InvalidateRect( hwnd, NULL, FALSE );
			return DefWindowProc( hwnd, message, wParam, lParam );
			break;
	}
	
	// デフォルト
	return DefWindowProc( hwnd, message, wParam, lParam);
}
