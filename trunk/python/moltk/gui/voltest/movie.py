from interpolate import CatmullRomSpline
import camera
from rotation import Vec3
from PySide.QtCore import QElapsedTimer, QCoreApplication, QXmlStreamReader

class Movie():
    def __init__(self, frames_per_second=24.0):
        self._key_frames = []
        # Create value arrays for convenient interpolation
        self._focusx = []
        self._focusy = []
        self._focusz = []
        self._zfv = []
        self._quat = []
        self.current_key_frame_index = None
        self.frames_per_second = frames_per_second
        self.spline = CatmullRomSpline()
        
    def __len__(self):
        return len(self._key_frames)
    
    def write_xml(self, writer):
        writer.writeStartElement("cinemol_movie_script")
        writer.writeAttribute("frames_per_second", str(self.frames_per_second))
        for key_frame in self._key_frames:
            key_frame.write_xml(writer)
        writer.writeEndElement()

    def read_xml(self, reader):
        self.frames_per_second = float(reader.attributes().value("frames_per_second"))
        reader.readNext()
        while not reader.tokenType() == QXmlStreamReader.EndElement:
            if reader.tokenType() == QXmlStreamReader.StartElement:
                if reader.name() == "key_frame":
                    camera_state = camera.State()
                    key_frame = KeyFrame(camera_state)
                    key_frame.read_xml(reader)
                    self.append(key_frame)
            reader.readNext()
        
    def append(self, key_frame):
        self._key_frames.append(key_frame)
        self._focusx.append(key_frame.camera_state.focus.x)
        self._focusy.append(key_frame.camera_state.focus.y)
        self._focusz.append(key_frame.camera_state.focus.z)
        self._zfv.append(key_frame.camera_state.zFocus_vheight)
        self._quat.append(key_frame.camera_state.rotation.to_quaternion())
        self.current_key_frame_index = len(self._key_frames) - 1
        
    def increment(self):
        if len(self._key_frames) < 1:
            self.current_key_frame_index = None
            return
        self.current_key_frame_index += 1
        if (self.current_key_frame_index) >= len(self._key_frames):
            self.current_key_frame_index = 0;
        
    def decrement(self):
        if len(self._key_frames) < 1:
            self.current_key_frame_index = None
            return
        self.current_key_frame_index -= 1
        if (self.current_key_frame_index) < 0:
            self.current_key_frame_index = len(self._key_frames) - 1;

    def clear(self):
        self.current_key_frame_index = None
        self._key_frames[:] = []
        self._focusx = []
        self._focusy = []
        self._focusz = []
        self._zfv = []
        self._quat = []

    def play(self, do_loop=False, real_time=True):
        time = 0.0 # seconds
        timer = QElapsedTimer()
        timer.restart()
        dtime = 1.0 / self.frames_per_second
        key_frame_number = 0
        frame_number = 0
        for kf in self._key_frames:
            self.current_key_frame_index = key_frame_number
            key_frame_time = 0.0
            while key_frame_time < kf.time_to_next_frame:
                # Drop frames if we are going too slow
                # if real_time and (timer.elapsed() - 200) > time*1000.0:
                #     continue
                tf = key_frame_time / kf.time_to_next_frame
                interpolation_parameter = tf + key_frame_number
                camera_state = camera.State()
                focus_x = self.spline.interpolate_sequence(
                            self._focusx, 
                            interpolation_parameter,
                            do_loop)
                focus_y = self.spline.interpolate_sequence(
                            self._focusy, 
                            interpolation_parameter,
                            do_loop)
                focus_z = self.spline.interpolate_sequence(
                            self._focusz, 
                            interpolation_parameter,
                            do_loop)
                camera_state.focus= Vec3([focus_x, focus_y, focus_z])
                camera_state.zFocus_vheight = self.spline.interpolate_sequence(
                            self._zfv,
                            interpolation_parameter,
                            do_loop)
                quat = self.spline.interpolate_quaternion_sequence(
                            self._quat,
                            interpolation_parameter,
                            do_loop)
                camera_state.rotation = quat.to_rotation()
                if real_time:
                    while timer.elapsed() < time*1000.0:
                        # TODO - use threading instead
                        QCoreApplication.processEvents()
                frame = KeyFrame(camera_state)
                frame.frame_number = frame_number + 1
                frame.key_frame_number = key_frame_number + 1
                yield frame
                key_frame_time += dtime
                time += dtime
                frame_number += 1
                if (not do_loop) and (key_frame_number == len(self._key_frames) - 1):
                    return # Don't go past final frame
            key_frame_number += 1
        
    @property
    def current_key_frame(self):
        if self.current_key_frame_index is None:
            return None
        return self._key_frames[self.current_key_frame_index]

class KeyFrame:
    def __init__(self, camera_state, time=2.5):
        self.camera_state = camera_state
        self.time_to_next_frame = time # seconds
        
    def write_xml(self, writer):
        writer.writeStartElement("key_frame")
        writer.writeAttribute("time_to_next_frame", str(self.time_to_next_frame))
        self.camera_state.write_xml(writer)
        writer.writeEndElement()
        
    def read_xml(self, reader):
        self.time_to_next_frame = float(reader.attributes().value("time_to_next_frame"))
        reader.readNext()
        while not reader.tokenType() == QXmlStreamReader.EndElement:
            if reader.tokenType() == QXmlStreamReader.StartElement:
                if reader.name() == "camera_position":
                    self.camera_state.read_xml(reader)
            reader.readNext()


