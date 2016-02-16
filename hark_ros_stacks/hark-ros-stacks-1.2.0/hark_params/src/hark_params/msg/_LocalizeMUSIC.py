"""autogenerated by genpy from hark_params/LocalizeMUSIC.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class LocalizeMUSIC(genpy.Message):
  _md5sum = "e153c85ee682045538315f87c1a9f5f0"
  _type = "hark_params/LocalizeMUSIC"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """int32 num_source
int32 min_deg
int32 max_deg
int32 lower_bound_frequency
int32 upper_bound_frequency

"""
  __slots__ = ['num_source','min_deg','max_deg','lower_bound_frequency','upper_bound_frequency']
  _slot_types = ['int32','int32','int32','int32','int32']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       num_source,min_deg,max_deg,lower_bound_frequency,upper_bound_frequency

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(LocalizeMUSIC, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.num_source is None:
        self.num_source = 0
      if self.min_deg is None:
        self.min_deg = 0
      if self.max_deg is None:
        self.max_deg = 0
      if self.lower_bound_frequency is None:
        self.lower_bound_frequency = 0
      if self.upper_bound_frequency is None:
        self.upper_bound_frequency = 0
    else:
      self.num_source = 0
      self.min_deg = 0
      self.max_deg = 0
      self.lower_bound_frequency = 0
      self.upper_bound_frequency = 0

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      _x = self
      buff.write(_struct_5i.pack(_x.num_source, _x.min_deg, _x.max_deg, _x.lower_bound_frequency, _x.upper_bound_frequency))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      end = 0
      _x = self
      start = end
      end += 20
      (_x.num_source, _x.min_deg, _x.max_deg, _x.lower_bound_frequency, _x.upper_bound_frequency,) = _struct_5i.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_struct_5i.pack(_x.num_source, _x.min_deg, _x.max_deg, _x.lower_bound_frequency, _x.upper_bound_frequency))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      end = 0
      _x = self
      start = end
      end += 20
      (_x.num_source, _x.min_deg, _x.max_deg, _x.lower_bound_frequency, _x.upper_bound_frequency,) = _struct_5i.unpack(str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_5i = struct.Struct("<5i")
