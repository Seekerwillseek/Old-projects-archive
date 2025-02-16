from tkinter import *
from tkinter.filedialog import askopenfilename, asksaveasfilename
from PIL import Image, ImageTk, ImageOps 
import os


def select_image():
    global prev_states # declare the global variable to store previous states
    prev_states = []
    file_path = askopenfilename(title="Select Image", filetypes=[("Image files", "*.jpg *.jpeg *.png")])
    
    if file_path:
        img = Image.open(file_path)
        show_image(img, file_path)



def image_reselect(img_label, img_window):
    print("Image is renamed")
    global prev_states # access the global variable
    prev_states.append(img_label.image)
    file_path1 = askopenfilename()
    if file_path1:
        reimg = Image.open(file_path1)
        show_image(reimg,file_path1 )


def image_delete(img_label, img_window, file_path):
    print("Image is deleted")
    global prev_states # access the global variable
    prev_states.append(img_label.image)
    photo_img = img_label.image
    os.remove(file_path)
    empty_photo = PhotoImage()
    img_label.config(image=empty_photo)
    img_label.image = empty_photo


def save_image(img_label, img_window):
    print("Image is saved")
    global prev_states # access the global variable
    prev_states.append(img_label.image)
    photo_img = img_label.image
    img = ImageTk.getimage(photo_img)
    file_path = asksaveasfilename(title="Save Image", defaultextension=".png",
                                  filetypes=[("PNG files", "*.png"), ("JPEG files", "*.jpg"), ("All files", "*.*")])
    if file_path:
        # Save the image to the specified file path
        img.save(file_path)
    photo_saved = ImageTk.PhotoImage(img)
    img_label.config(image=photo_saved)
    img_label.image = photo_saved

def addborder(img_label, img_window):
    print(" add border logic")
    global prev_states # access the global variable
    prev_states.append(img_label.image)
    photo_img = img_label.image
    img = ImageTk.getimage(photo_img)
    img_with_border = ImageOps.expand(img, border=10, fill='red')
    photo_img_with_border = ImageTk.PhotoImage(img_with_border)
    img_label.config(image=photo_img_with_border)
    img_label.image = photo_img_with_border


def change_filter(img_label, img_window):
    print("filter is added")
    global prev_states # access the global variable
    prev_states.append(img_label.image)
    photo_img = img_label.image
    img = ImageTk.getimage(photo_img)
    img_with_filter = img.convert("L")
    photo_img_with_filter = ImageTk.PhotoImage(img_with_filter)
    img_label.config(image=photo_img_with_filter)
    img_label.image = photo_img_with_filter


# def add_doodle(img_label, img_window):
#     print("Doodle is working")


def img_crop(img_label, img_window):
    global prev_states # access the global variable
    prev_states.append(img_label.image)
    photo_img = img_label.image
    img = ImageTk.getimage(photo_img)
    rotation_angle = getattr(img_label, 'rotation_angle', 0)
    rotated_img = img.rotate(-rotation_angle)
    cropped_img = rotated_img.crop((0, 0, rotated_img.width*0.8, rotated_img.height*0.8))
    rotated_cropped_img = cropped_img.rotate(rotation_angle)
    resized_rotated_cropped_img = rotated_cropped_img#.resize((img_label.winfo_width(), img_label.winfo_height()))
    photo_rotated_cropped_img = ImageTk.PhotoImage(resized_rotated_cropped_img)
    img_label.config(image=photo_rotated_cropped_img)
    img_label.image = photo_rotated_cropped_img


def img_rotate(img_label, img_window):
    global prev_states # access the global variable
    prev_states.append(img_label.image)
    photo_img = img_label.image
    img = ImageTk.getimage(photo_img)
    if not hasattr(img_label, 'rotation_angle'):
        img_label.rotation_angle = 0
    rotated_img = img.rotate(img_label.rotation_angle + 90)
    resized_rotated_img = rotated_img#.resize((img_label.winfo_width(), img_label.winfo_height()))
    photo_rotated_img = ImageTk.PhotoImage(resized_rotated_img)
    img_label.config(image=photo_rotated_img)
    img_label.image = photo_rotated_img


def undo(img_label):
    global prev_states # access the global variable
    if len(prev_states) > 0:
        prev_state = prev_states.pop() # remove the last state from the list
        img_label.config(image=prev_state) # update the image label widget with the previous state
        img_label.image = prev_state


def show_image(img, file_path):
    
    img_window = Toplevel()
    img_window.configure(bg='Black')
    img_window.title("Selected Image")
    img_window.attributes("-fullscreen", True)
    resized_img = img
    photo_img = ImageTk.PhotoImage(resized_img)
    img_label = Label(img_window, image=photo_img)
    img_label.image = photo_img
    img_label.rotation_angle = 0  
    img_label.place(relx=0.5, rely=0.5, anchor=CENTER)

    # Create a button to display next to the image
    btncrop = Button(img_window, text="   Crop   ", font=("Algerian", 14), bg='#9400D3', fg="white", command=lambda: img_crop(img_label, img_window))
    btncrop.place(relx=0.9, rely=0.2, anchor=CENTER)
    btnrotate = Button(img_window, text=" Rotate ", font=("Algerian", 14), bg='#9400D3', fg="white",command=lambda: img_rotate(img_label, img_window))
    btnrotate.place(relx=0.9,rely=0.3, anchor=CENTER)
    btnborder = Button(img_window, text=" Border ", font=("Algerian", 14), bg='#9400D3', fg="white", command=lambda : addborder(img_label, img_window))
    btnborder.place(relx=0.9,rely=0.4, anchor=CENTER)
    btnfilter = Button(img_window, text="  filter   ", font=("Algerian", 14), bg='#9400D3', fg="white", command=lambda : change_filter(img_label, img_window))
    btnfilter.place(relx=0.9,rely=0.5, anchor=CENTER)
    close_button = Button(img_window, text="  Close   ", font=("Algerian", 14), bg='#9400D3', fg="white", command=img_window.destroy)
    close_button.place(relx=0.9, rely=0.6, anchor=CENTER)
    btnsave = Button(img_window, text="    Save   ", font=("Algerian", 14), bg='#9400D3', fg="white", command=lambda : save_image(img_label, img_window))
    btnsave.place(relx=0.1,rely=0.2, anchor=CENTER)
    btnsave = Button(img_window, text="Re-select", font=("Algerian", 14), bg='#9400D3', fg="white", command=lambda : image_reselect(img_label, img_window))
    btnsave.place(relx=0.1,rely=0.3, anchor=CENTER)
    btnsave = Button(img_window, text="  Delete  ", font=("Algerian", 14), bg='#9400D3', fg="white", command=lambda : image_delete(img_label, img_window, file_path))
    btnsave.place(relx=0.1,rely=0.4, anchor=CENTER)
    btnundo = Button(img_window, text="   Undo   ", font=("Algerian", 14), bg='#9400D3', fg="white",command=lambda: undo(img_label))
    btnundo.place(relx=0.1,rely=0.5, anchor=CENTER)
    # btnundo = Button(img_window, text="    Undo    ", font=("Algerian", 14), bg='#9400D3', fg="white",command=lambda: undo(img_label))
    # btnundo.place(relx=0.1,rely=0.6, anchor=CENTER)
    
    # Start the event loop for the image window
    img_window.mainloop()
    
root = Tk()
root.title("Welcome to our ImagiNation")
background_image = Image.open("bg.jpg")
resized_background_image = background_image.resize((root.winfo_screenwidth(), root.winfo_screenheight()))
background_photo = ImageTk.PhotoImage(resized_background_image)
background_label = Label(root, image=background_photo)
background_label.place(x=0, y=0, relwidth=1, relheight=1)
image1 = Image.open("firstpage.jpg")
resized_img1 = image1.resize((int(image1.width*0.70), int(image1.height*0.70)))
photo_img1 = ImageTk.PhotoImage(resized_img1)
label1 = Label(root, image=photo_img1)
label1.place(relx=0.5, rely=0, anchor='n')
welcome_label = Label(root, text="            Welcome to our ImagiNation!            ", font=("Algerian", 35))
welcome_label.place(relx=0.5,rely=0.8, anchor=CENTER)
select_button = Button(root, text="    Select     ", font=("Algerian", 14),bg='#9400D3', fg='white', command=select_image)
select_button.place(relx=0.4,rely=0.9, anchor=CENTER)
close_button = Button(root, text="       Exit      ", font=("Algerian", 14),bg='#9400D3', fg='white', command=root.destroy)
close_button.place(relx=0.6,rely=0.9, anchor=CENTER)
root.attributes("-fullscreen", True)
root.mainloop()