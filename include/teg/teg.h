///  Copyright (c) 2024 Adrian & Leonardo Larrad.
///  
///  This software is provided 'as-is', without any express or implied warranty. 
///  In no event will the authors be held liable for any damages arising from
///  the use of this software.
///  
///  Permission is granted to anyone to use this software for any purpose,including
///  commercial applications, and to alter it and redistribute it freely, subject
///  to the following restrictions:
///  
///  1. The origin of this software must not be misrepresented; you must not claim
///     that you wrote the original software. If you use this software in a product, 
///     an acknowledgment in the product documentation would be appreciated but is 
///     not required.
///  2. Altered source versions must be plainly marked as such, and must not be
///     misrepresented as being the original software.
///  3. This notice may not be removed or altered from any source distribution.

#ifndef TEG_H
#define TEG_H

#include "./def.h"
#include "./md5.h"
#include "./fixed_string.h"

///  Reflection Module.
#include "./core_concepts.h"
#include "./container_concepts.h"
#include "./members_count.h"
#include "./members_equal.h"
#include "./members_get.h"
#include "./members_tie.h"
#include "./members_visitor.h"
#include "./alignment.h"
#include "./index_table.h"
#include "./constexpr_compat.h"

///  Varint Module.
#include "./varint.h"

///  Serialization Module.
#include "./version.h"
#include "./buffer.h"
#include "./c_array.h"
#include "./compatible.h"
#include "./decoder.h"
#include "./encoder.h"
#include "./endian.h"
#include "./error.h"
#include "./options.h"
#include "./schema.h"
#include "./serialization_concepts.h"
#include "./serialization.h"
#include "./deserialization.h"
#include "./unreachable.h"
#include "./varint_serialization.h"

#endif // TEG_H
