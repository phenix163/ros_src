"""autogenerated by genpy from hark_msgs/HarkSrcFeatureVal.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class HarkSrcFeatureVal(genpy.Message):
  _md5sum = "9387f3e7ecba46a9afd4e4e24e4df8bd"
  _type = "hark_msgs/HarkSrcFeatureVal"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """int32 id
float32 power
float32 x
float32 y
float32 z
float32 theta
int32 length
int32 data_bytes
float32[] featuredata

"""
  __slots__ = ['id','power','x','y','z','theta','length','data_bytes','featuredata']
  _slot_types = ['int32','float32','float32','float32','float32','float32','int32','int32','float32[]']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       id,power,x,y,z,theta,length,data_bytes,featuredata

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(HarkSrcFeatureVal, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.id is None:
        self.id = 0
      if self.power is None:
        self.power = 0.
      if self.x is None:
        self.x = 0.
      if self.y is None:
        self.y = 0.
      if self.z is None:
        self.z = 0.
      if self.theta is None:
        self.theta = 0.
      if self.length is None:
        self.length = 0
      if self.data_bytes is None:
        self.data_bytes = 0
      if self.featuredata is None:
        self.featuredata = []
    else:
      self.id = 0
      self.power = 0.
      self.x = 0.
      self.y = 0.
      self.z = 0.
      self.theta = 0.
      self.length = 0
      self.data_bytes = 0
      self.featuredata = []

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
      buff.write(_struct_i5f2i.pack(_x.id, _x.power, _x.x, _x.y, _x.z, _x.theta, _x.length, _x.data_bytes))
      length = len(self.featuredata)
      buff.write(_struct_I.pack(length))
      pattern = '<%sf'%length
      buff.write(struct.pack(pattern, *self.featuredata))
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
      end += 32
      (_x.id, _x.power, _x.x, _x.y, _x.z, _x.theta, _x.length, _x.data_bytes,) = _struct_i5f2i.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sf'%length
      start = end
      end += struct.calcsize(pattern)
      self.featuredata = struct.unpack(pattern, str[start:end])
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
      buff.write(_struct_i5f2i.pack(_x.id, _x.power, _x.x, _x.y, _x.z, _x.theta, _x.length, _x.data_bytes))
      length = len(self.featuredata)
      buff.write(_struct_I.pack(length))
      pattern = '<%sf'%length
      buff.write(self.featuredata.tostring())
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
      end += 32
      (_x.id, _x.power, _x.x, _x.y, _x.z, _x.theta, _x.length, _x.data_bytes,) = _struct_i5f2i.unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sf'%length
      start = end
      end += struct.calcsize(pattern)
      self.featuredata = numpy.frombuffer(str[start:end], dtype=numpy.float32, count=length)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_i5f2i = struct.Struct("<i5f2i")
