%module rib

%{
#include "../utils.h"
#include "rib.h"
#include "rib_builtin_obj.h"
#include "rib_remote.h"
#include "rib_obj.h"
#include "cdap_provider.h"
#include "rib_local.h"
#include "rib_conn.h"
#include "rib_schema.h"
%}

%include "../utils.h"
%include "rib.h"
%include "rib_builtin_obj.h"
%include "rib_remote.h"
%include "rib_obj.h"
%include "cdap_provider.h"
%include "rib_local.h"
%include "rib_conn.h"
%include "rib_schema.h"
