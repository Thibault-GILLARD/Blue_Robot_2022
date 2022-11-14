
from kivy.app import App
# GridLayout arranges children in a matrix.
from kivy.uix.gridlayout import GridLayout
# Label is used to label something
from kivy.uix.label import Label
# used to take input from users
from kivy.uix.textinput import TextInput
# If we will not import this module
# It will through the error
from kivy.uix.slider import Slider
# Property that represents a numeric value
# within a minimum bound and / or maximum
# bound – within a numeric range.
from kivy.properties import NumericProperty
from kivy.lang import Builder
from kivymd.app import MDApp
from kivy.uix.button import Button
from kivymd.uix.button import MDRectangleFlatIconButton
from kivymd.uix.button import MDRectangleFlatButton
from kivymd.uix.button import MDFillRoundFlatButton
from kivymd.uix.button import MDRaisedButton
from kivymd.uix.button import MDFlatButton


# PORT COM
import serial
import time
# Settings
SerialObj = serial.Serial('COM14')
SerialObj.baudrate = 115200
SerialObj.bytesize = 8
SerialObj.parity = serial.PARITY_NONE
SerialObj.stopbits = 1
# INFOS
print("\nChanged Settings")
print('Port =', SerialObj.port)
print('Baudrate =', SerialObj.baudrate)
print('ByteSize =', SerialObj.bytesize)
print('Parity = ', SerialObj.parity)
print('StopBits =', SerialObj.stopbits)

# class in which we are defining the
# sliders and its effects


class LoginScreen(GridLayout):
    def __init__(self, **kwargs):

        super(LoginScreen, self).__init__(**kwargs)
        # super function can be used to gain access
        # to inherited methods from a parent or sibling class
        # that has been overwritten in a class object.
        self.cols = 6
        # Default values of x, y, z
        self.x_send = "% d" % 0
        self.y_send = "% d" % 0
        self.z_send = "% d" % 0
        self.speed_send = "% d" % 0
        self.acc_send = "% d" % 0

        # Hole to creat space-------------
        for i in range(6):
            self.Holeb0 = MDFlatButton(text='')
            self.add_widget(self.Holeb0)

        # Title--------------
        self.Title = MDFlatButton(text='      Control Test', font_size="25sp")
        self.add_widget(self.Title)

        # Ping--------------
        self.Send_Ping = MDRaisedButton(text="      PING      ", icon='check-circle', md_bg_color=(
            0.58, 0.65, 1, 0.7),  on_press=self.callback_press_btn_ping)
        self.add_widget(self.Send_Ping)

        # ID--------------
        self.Send_ID = MDRaisedButton(text="      ID ?      ", md_bg_color=(
            0.58, 0.65, 1, 0.7), on_press=self.callback_press_btn_ID)
        self.add_widget(self.Send_ID)

        # Hole 0-------------
        self.Hole_0 = MDFlatButton(text='')
        self.add_widget(self.Hole_0)
        self.Holeb0 = MDFlatButton(text='')
        self.add_widget(self.Holeb0)
        self.Holeb0 = MDFlatButton(text='')
        self.add_widget(self.Holeb0)

        # Hole to creat space-------------
        for i in range(6):
            self.Holeb0 = MDFlatButton(text='-')
            self.add_widget(self.Holeb0)

        # 2nd row - one label for caption,
        # one label for slider value
        self.X_Value = MDFlatButton(text='        X value :'+'0', font_size="20sp",
                                    theme_text_color="Custom", text_color=(1, 1, 1, 0.5))
        self.add_widget(self.X_Value)
        self.Y_Value = MDFlatButton(
            text='Y value :'+'0', font_size="20sp", theme_text_color="Custom", text_color=(1, 1, 1, 0.5))
        self.add_widget(self.Y_Value)
        self.Z_Value = MDFlatButton(
            text='Z value :'+'0', font_size="20sp", theme_text_color="Custom", text_color=(1, 1, 1, 0.5))
        self.add_widget(self.Z_Value)

        # HOME--------------
        self.Send_Home = MDRectangleFlatIconButton(
            text="HOME", icon='home', on_press=self.callback_press_btn_Home)
        self.add_widget(self.Send_Home)

        # Hole to creat space-------------
        for i in range(8):
            self.Holeb0 = MDFlatButton(text='')
            self.add_widget(self.Holeb0)

        # X------------------
        # declaring the slider and adding some effects to it
        self.X_Control = Slider(min=-375, max=375)
        # 1st row - one label, one slider
        self.add_widget(self.X_Control)
        # On the slider object Attach a callback
        # for the attribute named value
        self.X_Control.bind(value=self.on_value_x)

        # Y----------------
        self.Y_Control = Slider(min=-350, max=350, orientation='vertical')
        self.add_widget(self.Y_Control)
        self.Y_Control.bind(value=self.on_value_y)

        # Z ----------------
        self.Z_Control = Slider(min=-100, max=100)
        self.add_widget(self.Z_Control)
        self.Z_Control.bind(value=self.on_value_z)

        # MOVE--------------
        self.Send_Move = MDRaisedButton(text="Move To : 0 0 0", font_size="20sp", theme_text_color="Custom",
                                        text_color="white", on_press=self.callback_press_btn_move, pos_hint={"center_x": .5, "center_y": .5})
        self.add_widget(self.Send_Move)

        # Hole to creat space-------------
        for i in range(8):
            self.Holeb0 = MDFlatButton(text='')
            self.add_widget(self.Holeb0)

        # SOLENOIDS--------------

        # Solenoids_label--
        self.Solenoids_label = MDRectangleFlatIconButton(text='Solenoids -->', icon='swap-vertical', font_size="20sp",
                                                         theme_text_color="Custom", text_color="white", line_color=(0, 0, 0, 0), pos_hint={"center_x": .5, "center_y": .5})
        self.add_widget(self.Solenoids_label)

        # Solenoids_UP--
        self.Solenoids_Send_Up = MDRectangleFlatIconButton(text="UP", icon='arrow-up', pos_hint={
                                                           "center_x": .5, "center_y": .5}, on_press=self.callback_press_btn_solenoids_up)
        self.add_widget(self.Solenoids_Send_Up)

        # Solenoids_Down--
        self.Solenoids_Send_Down = MDRectangleFlatIconButton(text="DOWN", icon='arrow-down', pos_hint={
                                                             "center_x": .5, "center_y": .5}, on_press=self.callback_press_btn_solenoids_down)
        self.add_widget(self.Solenoids_Send_Down)

        # Solenoids_Stop--
        self.Solenoids_Send_Down = MDRectangleFlatIconButton(text="STOP", icon='pause', pos_hint={
                                                             "center_x": .5, "center_y": .5}, on_press=self.callback_press_btn_solenoids_stop)
        self.add_widget(self.Solenoids_Send_Down)

        # Hole to creat space-------------
        for i in range(8):
            self.Holeb0 = MDFlatButton(text='')
            self.add_widget(self.Holeb0)

        # VACUUM_BLAST--------------

        # Vac_Bla_label--
        self.Vac_Bla_label = MDRectangleFlatIconButton(text='Vacuum/Blast -->', icon='play-circle', font_size="20sp",
                                                       theme_text_color="Custom", text_color="white", line_color=(0, 0, 0, 0), pos_hint={"center_x": .5, "center_y": .5})
        self.add_widget(self.Vac_Bla_label)

        # Vacuum_On--
        self.Vacuum_On = MDRectangleFlatIconButton(text="Vacuum On", icon='play', pos_hint={
                                                   "center_x": .5, "center_y": .5}, on_press=self.callback_press_btn_vacuum_on)
        self.add_widget(self.Vacuum_On)

        # Blast_On--
        self.Blast_On = MDRectangleFlatIconButton(text="Blast On", icon='play', pos_hint={
                                                  "center_x": .5, "center_y": .5}, on_press=self.callback_press_btn_blasr_on)
        self.add_widget(self.Blast_On)

        # Vac_Bla_Off--
        self.Vac_Bla_Off = MDRectangleFlatIconButton(text="Vac/Bla Off", icon='pause', pos_hint={
                                                     "center_x": .5, "center_y": .5}, on_press=self.callback_press_btn_vac_bla_off)
        self.add_widget(self.Vac_Bla_Off)

        # Hole to creat space-------------
        for i in range(8):
            self.Holeb0 = MDFlatButton(text='')
            self.add_widget(self.Holeb0)

        # INFO_LABEL--
        self.Info_label = MDRectangleFlatIconButton(text='Info -->', icon='help', font_size="20sp", theme_text_color="Custom",
                                                    text_color="white", line_color=(0, 0, 0, 0), pos_hint={"center_x": .5, "center_y": .5})
        self.add_widget(self.Info_label)

        # ROFFSET--------------
        self.Send_Roffset = MDRaisedButton(text="Roffset", md_bg_color=(
            0.58, 0.65, 1, 0.7), on_press=self.callback_press_btn_roffset)
        self.add_widget(self.Send_Roffset)

        # Hole Roffset-------------
        self.Hole_1 = MDFlatButton(text='')
        self.add_widget(self.Hole_1)

        # Prox--------------
        self.Send_Prox = MDRaisedButton(text="Prox", md_bg_color=(
            0.58, 0.65, 1, 0.8), on_press=self.callback_press_btn_prox)
        self.add_widget(self.Send_Prox)

        # Hole Prox-------------
        self.Hole_2 = MDFlatButton(text='')
        self.add_widget(self.Hole_2)

        # Hole to creat space-------------
        for i in range(13):
            self.Holeb0 = MDFlatButton(text='')
            self.add_widget(self.Holeb0)

        # SPEED_ACC
        # SPEED_LABEL--
        self.Speed_label = MDRectangleFlatIconButton(text='Speed :', icon='fast-forward', font_size="20sp", theme_text_color="Custom",
                                                     text_color="white", line_color=(0, 0, 0, 0), pos_hint={"center_x": .5, "center_y": .5})
        self.add_widget(self.Speed_label)

        # Acc_LABEL--
        self.Acc_label = MDRectangleFlatIconButton(text='Acceleration :', icon='fast-forward', font_size="20sp",
                                                   theme_text_color="Custom", text_color="white", line_color=(0, 0, 0, 0), pos_hint={"center_x": .5, "center_y": .5})
        self.add_widget(self.Acc_label)

        # Send_Speed--------------
        self.Send_speed_acc = MDRaisedButton(text="Speed/Acc : 0 0 %", font_size="20sp", md_bg_color=(0.58, 0.65, 1, 0.7), theme_text_color="Custom",
                                             text_color="white", on_press=self.callback_press_btn_speed_acc, pos_hint={"center_x": .5, "center_y": .5})
        self.add_widget(self.Send_speed_acc)

        # Hole to creat space-------------
        for i in range(3):
            self.Holeb0 = MDFlatButton(text='')
            self.add_widget(self.Holeb0)

        # Speed----------------
        self.Speed_Control = Slider(min=-100, max=100)
        self.add_widget(self.Speed_Control)
        self.Speed_Control.bind(value=self.on_value_speed)

        # Acc----------------
        self.Acc_Control = Slider(min=-100, max=100)
        self.add_widget(self.Acc_Control)
        self.Acc_Control.bind(value=self.on_value_acc)

        # Hole to creat space-------------
        for i in range(4):
            self.Holeb0 = MDFlatButton(text='')
            self.add_widget(self.Holeb0)

    # PING-----

    def callback_press_btn_ping(self, instance):
        if (print_out('PING') == b'PONG\r\n'):
            self.Send_Ping.text = 'PONG'
        else:
            self.Send_Ping.text = 'No PONG'

    # ID-----
    def callback_press_btn_ID(self, instance):
        if (print_out('ID') == b'ID, ROBOTL\r\n'):
            self.Send_ID.text = 'ID : ' + 'ROBOTL'
        elif (print_out('ID') == b'ID, ROBOTR\r\n'):
            self.Send_ID.text = 'ID : ' + 'ROBOTR'
        else:
            self.Send_ID.text = 'ID : No Signal'
    # HOME-----

    def callback_press_btn_Home(self, instance):
        print_out('HOME')

    # ROFFSET-----
    def callback_press_btn_roffset(self, instance):
        self.Hole_1.text = str(print_out('ROFFSET')[0:-2])

    # Prox-----
    def callback_press_btn_prox(self, instance):
        self.Hole_2.text = str(print_out('PROX')[0:-2])

    # MOVE-----
    def on_value_x(self, instance, x_position):
        self.X_Value.text = '        X value : ' + \
            "% d" % x_position  # Label's text changement
        self.x_send = "% d" % x_position  # Save the new x position
        self.Send_Move.text = 'Move To : ' + self.x_send + \
            self.y_send + self.z_send  # Button's text changement
        self.Send_Ping.text = 'PING'

    def on_value_y(self, instance, y_position):
        self.Y_Value.text = 'Y value : '+"% d" % y_position
        self.y_send = "% d" % y_position
        self.Send_Move.text = 'Move To : ' + self.x_send + self.y_send + self.z_send
        self.Send_Ping.text = 'PING'

    def on_value_z(self, instance, z_position):
        self.Z_Value.text = 'Z value : '+"% d" % z_position
        self.z_send = "% d" % z_position
        self.Send_Move.text = 'Move To : ' + self.x_send + self.y_send + self.z_send
        self.Send_Ping.text = 'PING'

    def callback_press_btn_move(self, *args):
        print(self.x_send)
        print(self.y_send)
        print(self.z_send)
        print_out_move(self.x_send, self.y_send, self.z_send)
        self.Send_Ping.text = 'PING'

    def callback_press_btn_solenoids_up(self, instance):
        print_out('AIR,U')
        self.Send_Ping.text = 'PING'

    def callback_press_btn_solenoids_down(self, instance):
        print_out('AIR,D')
        self.Send_Ping.text = 'PING'

    def callback_press_btn_solenoids_stop(self, instance):
        print_out('AIR,S')
        self.Send_Ping.text = 'PING'

    def callback_press_btn_vacuum_on(self, instance):
        print_out('AIR,V')
        self.Send_Ping.text = 'PING'

    def callback_press_btn_blasr_on(self, instance):
        print_out('AIR,B')
        self.Send_Ping.text = 'PING'

    def callback_press_btn_vac_bla_off(self, instance):
        print_out('AIR,O')
        self.Send_Ping.text = 'PING'

    # Speed acc
    def on_value_speed(self, instance, speed_position):
        self.speed_send = "% d" % speed_position
        self.Send_speed_acc.text = 'Speed/Acc : ' + self.speed_send + \
            self.acc_send + '%'  # Button's text changement
        self.Send_Ping.text = 'PING'

    def on_value_acc(self, instance, acc_position):
        self.acc_send = "% d" % acc_position
        self.Send_speed_acc.text = 'Speed/Acc : ' + self.speed_send + \
            self.acc_send + '%'  # Button's text changement
        self.Send_Ping.text = 'PING'

    def callback_press_btn_speed_acc(self, *args):
        print(self.speed_send)
        print(self.acc_send)
        move_str = 'SPEEDSET,' + self.speed_send + ',' + self.acc_send
        print_out(move_str)
        self.Send_Ping.text = 'PING'


def print_out(message):
    msg = message
    mdg_str_encode = str(msg).encode('ascii')
    SerialObj.write(mdg_str_encode)
    RecivedStr = SerialObj.readline()
    print(RecivedStr)
    return  RecivedStr


def print_out_move(value_x, value_y, value_z):
    move_str = 'MOVE,' + value_x + ',' + value_y + ',' + value_z
    move_str_encode = str(move_str).encode('ascii')
    # SerialObj.write(b'1,2\r')
    SerialObj.write(move_str_encode)
    RecivedStr = SerialObj.readline()
    print(RecivedStr)


# the Base Class of our Kivy App
class MyApp(MDApp):
    def build(self):
        # return a LoginScreen() as a root widget
        self.theme_cls.theme_style_switch_animation = True
        self.theme_cls.theme_style = "Dark"
        self.theme_cls.primary_palette = "Orange"
        self.title = 'Control Test ;)'
        return LoginScreen()


if __name__ == '__main__':
    MyApp().run()
