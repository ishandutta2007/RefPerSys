// see http://refpersys.org/
// SPDX-License-Identifier: GPL-3.0-or-later
// GPLv3+ licensed file plugins_dir/rpsplug_create_cplusplus_primitive_type.cc
// © Copyright 2024 - 2025 Basile Starynkevitch <basile@starynkevitch.net>
// This plugin creates a new RefPerSys C++ primitive type for C++ generation
// and could be used by machine (GNU lightning) or libgccjit code generation
/*****
      Once compiled, use it for example as:
      ./refpersys --plugin-after-load=/tmp/rpsplug_create_cplusplus_primitive_type.so \
      --plugin-arg=rpsplug_create_cplusplus_create_primitive_type:native_int_type \
      --extra=comment='the native int type' \
      --batch --dump=.

then update appropriately the rps_set_native_data_in_loader to force
native alignement and size
****/




#include "refpersys.hh"

#warning incomplete plugins_dir/rpsplug_create_cplusplus_primitive_type.cc
void
rps_do_plugin(const Rps_Plugin* plugin)
{
  RPS_LOCALFRAME(/*descr:*/nullptr, /*callerframe:*/nullptr,
                           Rps_ObjectRef obmutsetclass;
                           Rps_ObjectRef obsuperclass;
                           Rps_ObjectRef obcppcodeclass;
                           Rps_ObjectRef obcpptype;
                           Rps_ObjectRef obsymbol;
                           Rps_Value namestr; // a string
                           Rps_Value commentstr; // a string
                );
  const char*plugarg = rps_get_plugin_cstr_argument(plugin);
  const char*comment = rps_get_extra_arg("comment");
  const char*instanced = rps_get_extra_arg("instance");
  bool isinstanced = false;
  RPS_INFORMOUT("loaded plugin " <<  plugin->plugin_name
		<< " file " << __FILE__
		<< " comment:" << Rps_QuotedC_String(comment)
		<< " instance:" << Rps_QuotedC_String(instanced)
		<< " plugarg:" << Rps_QuotedC_String(plugarg)
		<< std::endl
		<< RPS_FULL_BACKTRACE_HERE(1, "rps_do_plugin/createC++primtyp"));
  _f.obcppcodeclass = RPS_ROOT_OB(_14J2l2ZPGtp00WLhIu); //cplusplus_code∈class
  if (!plugarg || plugarg[0]==(char)0)
    RPS_FATALOUT("failure: plugin/createC++primtyp " << plugin->plugin_name
                 << " without argument; should be some non-empty name");
  RPS_POSSIBLE_BREAKPOINT();
  ///
  /* Check that plugarg is a good name */
  {
    bool goodplugarg = isalpha(plugarg[0]);
    for (const char*pa = &plugarg[0]; goodplugarg && *pa; pa++)
      goodplugarg = isalnum(*pa) || *pa=='_';
  }
  if (instanced)
    {
      if (!strcmp(instanced, "true")) isinstanced = true;
      if (instanced[0]=='y' || instanced[0] == 'Y') isinstanced = true;
      if (atoi(instanced) > 0) isinstanced = true;
    };
  /* Check that plugarg is some new name */
  if (auto nob = Rps_ObjectRef::find_object_or_null_by_string(&_, std::string(plugarg)))
    {
      RPS_FATALOUT("failure: plugin " << plugin->plugin_name << " argument " << plugarg
                   << " is naming an existing object " << nob);
    };
  RPS_POSSIBLE_BREAKPOINT();
  _f.obcpptype = Rps_ObjectRef::make_object(&_,
					    /*class:*/_f.obcppcodeclass,
					    /*space:*/Rps_ObjectRef::root_space()
					    );
  if (comment)
    {
      _f.commentstr = Rps_StringValue(comment);
      _f.obcpptype->put_attr(RPS_ROOT_OB(_0jdbikGJFq100dgX1n), //comment∈symbol
                              _f.commentstr);
    }
  _f.namestr = Rps_Value{std::string(plugarg)};
  /// Avoid using below RPS_ROOT_OB(4FBkYDlynyC02QtkfG):"name"∈named_attribute
  /// it was was a mistake.
  _f.obcpptype
  ->put_attr(RPS_ROOT_OB(_1EBVGSfW2m200z18rx), //name∈named_attribute
             _f.namestr);
  /* Create a symbol for the new class name. */
  _f.obsymbol = Rps_ObjectRef::make_new_strong_symbol(&_, std::string{plugarg});
  std::lock_guard<std::recursive_mutex> gusymbol(*(_f.obsymbol->objmtxptr()));
  Rps_PayloadSymbol* paylsymb = _f.obsymbol->get_dynamic_payload<Rps_PayloadSymbol>();
  RPS_ASSERT (paylsymb != nullptr);
  paylsymb->symbol_put_value(_f.obcpptype);
  _f.obcpptype->put_attr(RPS_ROOT_OB(_3Q3hJsSgCDN03GTYW5), //symbol∈symbol
                          _f.obsymbol);
  if (isinstanced)
    {
      RPS_WARNOUT("rpsplug_create_cplusplus_code_class added new object  "
                  << _f.obcpptype
                  << " named " << plugarg << " of class "
                  << _f.obcppcodeclass << " and symbol " << _f.obsymbol
                  << " in space " << _f.obcpptype->get_space());
    }
  else
    {
      RPS_INFORMOUT("rpsplug_create_cplusplus_code_class added new object " << _f.obcpptype
                    << " named " << plugarg << " of super class "
                    << _f.obcppcodeclass << " and symbol " << _f.obsymbol
                    << " in space " << _f.obcpptype->get_space());
    }
} // end rps_do_plugin


/****************
 **                           for Emacs...
 ** Local Variables: ;;
 ** compile-command: "cd ..; make plugins_dir/rpsplug_create_cplusplus_primitive_type.so &&  /bin/ln -sfv $(/bin/pwd)/plugins_dir/rpsplug_create_cplusplus_primitive_type.so /tmp/"" ;;
 ** End: ;;
 ****************/
