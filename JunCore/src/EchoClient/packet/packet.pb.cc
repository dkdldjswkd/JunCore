// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: packet.proto



#pragma warning(push)
#pragma warning(disable:4125) // decimal digit terminates octal escape sequence
#pragma warning(disable:4800)
#undef MOBILE
#define GOOGLE_PROTOBUF_NO_RTTI 1
#define PROTOBUF_DISABLE_LITTLE_ENDIAN_OPT_FOR_TEST
#define GOOGLE_PROTOBUF_USE_UNALIGNED 0
#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#endif


#include "packet.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)


#if PLATFORM_WINDOWS
#include "Windows/HideWindowsPlatformTypes.h"
#endif //PLATFORM_WINDOWS


#include <google/protobuf/port_def.inc>
namespace PacketLib {
class UG_ECHO_REQDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<UG_ECHO_REQ> _instance;
} _UG_ECHO_REQ_default_instance_;
class GU_ECHO_RESDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<GU_ECHO_RES> _instance;
} _GU_ECHO_RES_default_instance_;
}  // namespace PacketLib
static void InitDefaultsscc_info_GU_ECHO_RES_packet_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::PacketLib::_GU_ECHO_RES_default_instance_;
    new (ptr) ::PacketLib::GU_ECHO_RES();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::PacketLib::GU_ECHO_RES::InitAsDefaultInstance();
}

VIC_PROTOBUF_EXPORT ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_GU_ECHO_RES_packet_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_GU_ECHO_RES_packet_2eproto}, {}};

static void InitDefaultsscc_info_UG_ECHO_REQ_packet_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::PacketLib::_UG_ECHO_REQ_default_instance_;
    new (ptr) ::PacketLib::UG_ECHO_REQ();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::PacketLib::UG_ECHO_REQ::InitAsDefaultInstance();
}

VIC_PROTOBUF_EXPORT ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_UG_ECHO_REQ_packet_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_UG_ECHO_REQ_packet_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_packet_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_packet_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_packet_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_packet_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::PacketLib::UG_ECHO_REQ, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::PacketLib::UG_ECHO_REQ, echo_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::PacketLib::GU_ECHO_RES, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::PacketLib::GU_ECHO_RES, echo_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::PacketLib::UG_ECHO_REQ)},
  { 6, -1, sizeof(::PacketLib::GU_ECHO_RES)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::PacketLib::_UG_ECHO_REQ_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::PacketLib::_GU_ECHO_RES_default_instance_),
};

const char descriptor_table_protodef_packet_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\014packet.proto\022\tPacketLib\"\033\n\013UG_ECHO_REQ"
  "\022\014\n\004echo\030\001 \001(\t\"\033\n\013GU_ECHO_RES\022\014\n\004echo\030\001 "
  "\001(\tb\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_packet_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_packet_2eproto_sccs[2] = {
  &scc_info_GU_ECHO_RES_packet_2eproto.base,
  &scc_info_UG_ECHO_REQ_packet_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_packet_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_packet_2eproto = {
  false, false, descriptor_table_protodef_packet_2eproto, "packet.proto", 91,
  &descriptor_table_packet_2eproto_once, descriptor_table_packet_2eproto_sccs, descriptor_table_packet_2eproto_deps, 2, 0,
  schemas, file_default_instances, TableStruct_packet_2eproto::offsets,
  file_level_metadata_packet_2eproto, 2, file_level_enum_descriptors_packet_2eproto, file_level_service_descriptors_packet_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_packet_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_packet_2eproto)), true);
namespace PacketLib {

// ===================================================================

void UG_ECHO_REQ::InitAsDefaultInstance() {
}
class UG_ECHO_REQ::_Internal {
 public:
};

UG_ECHO_REQ::UG_ECHO_REQ(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:PacketLib.UG_ECHO_REQ)
}
UG_ECHO_REQ::UG_ECHO_REQ(const UG_ECHO_REQ& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  echo_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_echo().empty()) {
    echo_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_echo(),
      GetArena());
  }
  // @@protoc_insertion_point(copy_constructor:PacketLib.UG_ECHO_REQ)
}

void UG_ECHO_REQ::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_UG_ECHO_REQ_packet_2eproto.base);
  echo_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

UG_ECHO_REQ::~UG_ECHO_REQ() {
  // @@protoc_insertion_point(destructor:PacketLib.UG_ECHO_REQ)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void UG_ECHO_REQ::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  echo_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void UG_ECHO_REQ::ArenaDtor(void* object) {
  UG_ECHO_REQ* _this = reinterpret_cast< UG_ECHO_REQ* >(object);
  (void)_this;
}
void UG_ECHO_REQ::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void UG_ECHO_REQ::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const UG_ECHO_REQ& UG_ECHO_REQ::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_UG_ECHO_REQ_packet_2eproto.base);
  return *internal_default_instance();
}


void UG_ECHO_REQ::Clear() {
// @@protoc_insertion_point(message_clear_start:PacketLib.UG_ECHO_REQ)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  echo_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* UG_ECHO_REQ::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // string echo = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          auto str = _internal_mutable_echo();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "PacketLib.UG_ECHO_REQ.echo"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* UG_ECHO_REQ::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:PacketLib.UG_ECHO_REQ)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string echo = 1;
  if (this->echo().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_echo().data(), static_cast<int>(this->_internal_echo().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "PacketLib.UG_ECHO_REQ.echo");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_echo(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:PacketLib.UG_ECHO_REQ)
  return target;
}

size_t UG_ECHO_REQ::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:PacketLib.UG_ECHO_REQ)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string echo = 1;
  if (this->echo().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_echo());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void UG_ECHO_REQ::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:PacketLib.UG_ECHO_REQ)
  GOOGLE_DCHECK_NE(&from, this);
  const UG_ECHO_REQ* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<UG_ECHO_REQ>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:PacketLib.UG_ECHO_REQ)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:PacketLib.UG_ECHO_REQ)
    MergeFrom(*source);
  }
}

void UG_ECHO_REQ::MergeFrom(const UG_ECHO_REQ& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:PacketLib.UG_ECHO_REQ)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.echo().size() > 0) {
    _internal_set_echo(from._internal_echo());
  }
}

void UG_ECHO_REQ::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:PacketLib.UG_ECHO_REQ)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void UG_ECHO_REQ::CopyFrom(const UG_ECHO_REQ& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:PacketLib.UG_ECHO_REQ)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool UG_ECHO_REQ::IsInitialized() const {
  return true;
}

void UG_ECHO_REQ::InternalSwap(UG_ECHO_REQ* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  echo_.Swap(&other->echo_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}

::PROTOBUF_NAMESPACE_ID::Metadata UG_ECHO_REQ::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void GU_ECHO_RES::InitAsDefaultInstance() {
}
class GU_ECHO_RES::_Internal {
 public:
};

GU_ECHO_RES::GU_ECHO_RES(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:PacketLib.GU_ECHO_RES)
}
GU_ECHO_RES::GU_ECHO_RES(const GU_ECHO_RES& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  echo_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (!from._internal_echo().empty()) {
    echo_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), from._internal_echo(),
      GetArena());
  }
  // @@protoc_insertion_point(copy_constructor:PacketLib.GU_ECHO_RES)
}

void GU_ECHO_RES::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_GU_ECHO_RES_packet_2eproto.base);
  echo_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

GU_ECHO_RES::~GU_ECHO_RES() {
  // @@protoc_insertion_point(destructor:PacketLib.GU_ECHO_RES)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void GU_ECHO_RES::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  echo_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void GU_ECHO_RES::ArenaDtor(void* object) {
  GU_ECHO_RES* _this = reinterpret_cast< GU_ECHO_RES* >(object);
  (void)_this;
}
void GU_ECHO_RES::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void GU_ECHO_RES::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const GU_ECHO_RES& GU_ECHO_RES::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_GU_ECHO_RES_packet_2eproto.base);
  return *internal_default_instance();
}


void GU_ECHO_RES::Clear() {
// @@protoc_insertion_point(message_clear_start:PacketLib.GU_ECHO_RES)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  echo_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* GU_ECHO_RES::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // string echo = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          auto str = _internal_mutable_echo();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(::PROTOBUF_NAMESPACE_ID::internal::VerifyUTF8(str, "PacketLib.GU_ECHO_RES.echo"));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* GU_ECHO_RES::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:PacketLib.GU_ECHO_RES)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string echo = 1;
  if (this->echo().size() > 0) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_echo().data(), static_cast<int>(this->_internal_echo().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "PacketLib.GU_ECHO_RES.echo");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_echo(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:PacketLib.GU_ECHO_RES)
  return target;
}

size_t GU_ECHO_RES::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:PacketLib.GU_ECHO_RES)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string echo = 1;
  if (this->echo().size() > 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_echo());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void GU_ECHO_RES::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:PacketLib.GU_ECHO_RES)
  GOOGLE_DCHECK_NE(&from, this);
  const GU_ECHO_RES* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<GU_ECHO_RES>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:PacketLib.GU_ECHO_RES)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:PacketLib.GU_ECHO_RES)
    MergeFrom(*source);
  }
}

void GU_ECHO_RES::MergeFrom(const GU_ECHO_RES& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:PacketLib.GU_ECHO_RES)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.echo().size() > 0) {
    _internal_set_echo(from._internal_echo());
  }
}

void GU_ECHO_RES::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:PacketLib.GU_ECHO_RES)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void GU_ECHO_RES::CopyFrom(const GU_ECHO_RES& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:PacketLib.GU_ECHO_RES)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool GU_ECHO_RES::IsInitialized() const {
  return true;
}

void GU_ECHO_RES::InternalSwap(GU_ECHO_RES* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  echo_.Swap(&other->echo_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}

::PROTOBUF_NAMESPACE_ID::Metadata GU_ECHO_RES::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace PacketLib
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::PacketLib::UG_ECHO_REQ* Arena::CreateMaybeMessage< ::PacketLib::UG_ECHO_REQ >(Arena* arena) {
  return Arena::CreateMessageInternal< ::PacketLib::UG_ECHO_REQ >(arena);
}
template<> PROTOBUF_NOINLINE ::PacketLib::GU_ECHO_RES* Arena::CreateMaybeMessage< ::PacketLib::GU_ECHO_RES >(Arena* arena) {
  return Arena::CreateMessageInternal< ::PacketLib::GU_ECHO_RES >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>