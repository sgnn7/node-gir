#pragma once

#include <vector>
#include <v8.h>
#include <nan.h>
#include <glib.h>
#include <girepository.h>

namespace gir {

class GIRObject;

struct ObjectFunctionTemplate {
    char *type_name;
    GIObjectInfo *info;
    v8::Handle<v8::FunctionTemplate> function;
    GType type;
    char *namespace_;
};

struct MarshalData {
    GIRObject *that;
    char *event_name;
};

struct InstanceData {
    v8::Handle<v8::Value> instance;
    GIRObject *obj;
};


class GIRObject : public Nan::ObjectWrap {
  public:
    GIRObject() {};
    GIRObject(GIObjectInfo *info_, int n_params, GParameter *parameters);
    virtual ~GIRObject();

    GObject *obj;
    bool abstract;
    GIBaseInfo *info;

    static std::vector<InstanceData> instances;
    static std::vector<ObjectFunctionTemplate> templates;

    static v8::Handle<v8::Value> New(GObject *obj, GIObjectInfo *info);
    static v8::Handle<v8::Value> New(GObject *obj, GType t);
    static NAN_METHOD(New);

    static void Prepare(v8::Handle<v8::Object> target, GIObjectInfo *info);
    static void SetPrototypeMethods(v8::Local<v8::FunctionTemplate> t, char *name);
    static void RegisterMethods(v8::Handle<v8::Object> target, GIObjectInfo *info, const char *namespace_, v8::Handle<v8::FunctionTemplate> t);

    static void Initialize(v8::Handle<v8::Object> target, char *namespace_);

    static NAN_METHOD(CallMethod);
    static NAN_METHOD(CallUnknownMethod);
    static NAN_METHOD(GetProperty);
    static NAN_METHOD(SetProperty);
    static NAN_METHOD(GetInterface);
    static NAN_METHOD(GetField);
    static NAN_METHOD(WatchSignal);
    static NAN_METHOD(CallVFunc);

    static void PushInstance(GIRObject *obj, v8::Handle<v8::Value>);
    static v8::Handle<v8::Value> GetInstance(GObject *obj);

    static void SignalFinalize(gpointer data, GClosure *c);
    static void SignalCallback(GClosure *closure,
        GValue *return_value,
        guint n_param_values,
        const GValue *param_values,
        gpointer invocation_hint,
        gpointer marshal_data);
    v8::Handle<v8::Value> Emit(v8::Handle<v8::Value> argv[], int length);

    static GIFunctionInfo *FindMethod(GIObjectInfo *inf, char *name);
    static GIFunctionInfo *FindProperty(GIObjectInfo *inf, char *name);
    static GIFunctionInfo *FindInterface(GIObjectInfo *inf, char *name);
    static GIFunctionInfo *FindField(GIObjectInfo *inf, char *name);
    static GIFunctionInfo *FindSignal(GIObjectInfo *inf, char *name);
    static GIFunctionInfo *FindVFunc(GIObjectInfo *inf, char *name);

  private:
    static v8::Handle<v8::Object> PropertyList(GIObjectInfo *info);
    static v8::Handle<v8::Object> MethodList(GIObjectInfo *info);
    static v8::Handle<v8::Object> InterfaceList(GIObjectInfo *info);
    static v8::Handle<v8::Object> FieldList(GIObjectInfo *info);
    static v8::Handle<v8::Object> SignalList(GIObjectInfo *info);
    static v8::Handle<v8::Object> VFuncList(GIObjectInfo *info);

    static bool ToParams(v8::Handle<v8::Value> val, GParameter** p, int *length, GIObjectInfo *info);
    static void DeleteParams(GParameter* params, int length);
};

}
