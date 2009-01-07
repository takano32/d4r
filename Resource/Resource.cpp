#pragma comment (lib, "msvcrt-ruby18.lib")
#include <ruby.h>

#include "Resource.h"
#include "ResourceClass.h"
using namespace base;


struct ResourceData {
	ResourceClass* klass;
};

static void resource_dfree( struct ResourceData* data ) {
	delete data->klass;
}

static VALUE resource_alloc(VALUE klass) {
    struct ResourceData *data = ALLOC(struct ResourceData);
    return Data_Wrap_Struct(klass, 0, resource_dfree, data);
}

static VALUE resource_initialize( VALUE self, VALUE path ) {
    struct ResourceData *data;
	Data_Get_Struct(self, struct ResourceData, data);
	data->klass = new ResourceClass( StringValuePtr(path) );
    return Qnil;
}

static VALUE resource_read_in( VALUE self, VALUE resource_name ) {
	struct ResourceData *data;
	Data_Get_Struct(self, struct ResourceData, data);

	StringValue(resource_name);
	// メモリコピーの処理が余計になるが、Stringの中身を書き換えるのは
	// きちんとやるには多分スレッドセーフにしないといけなくて大変。
	unsigned int size = data->klass->GetFileSize( StringValuePtr(resource_name) );
	if( size == 0 ){
		rb_raise(rb_eIOError, "unable to read file: %s", StringValuePtr(resource_name));
		return Qnil;
	}
	char *memory_image = ALLOC_N(char, size);
	if(! data->klass->ReadIn( StringValuePtr(resource_name), (unsigned char*)memory_image, size ) ){
		rb_raise(rb_eIOError, "unable to read file: %s", StringValuePtr(resource_name));
		return Qnil;
	}
	VALUE retval = rb_str_new( memory_image, size );
	xfree(memory_image);
	return retval;
}

void Init_resource() {
    VALUE Resource;
	typedef VALUE (*ruby_method)(...);

    Resource = rb_define_class("Resource", rb_cObject);
    rb_define_alloc_func(Resource, resource_alloc);

	// param1:管理するアーカイブまたは、ディレクトリ
    rb_define_private_method(Resource, "initialize", (ruby_method)resource_initialize, 1);

	// param1:取り出したいファイルのファイル名
	// retval:成功の場合、メモリイメージ(String)、失敗の場合IOException
	rb_define_method(Resource, "get", (ruby_method)resource_read_in, 1);
}
