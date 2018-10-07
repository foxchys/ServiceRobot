import face_recognition


def facefounct(a):
    picture_of_me = face_recognition.load_image_file(a)
    my_face_encoding = face_recognition.face_encodings(picture_of_me)[0]

    unknown_picture = face_recognition.load_image_file("pic0.jpg")

    unknown_face_encoding = face_recognition.face_encodings(unknown_picture)[0]

    # Now we can see the two face encodings are of the same person with `compare_faces`!

    results = face_recognition.compare_faces([my_face_encoding], unknown_face_encoding)

    if results[0] == 1:
        return 1
    else:
        return 2


def main():
    asss = facefounct("2.jpg")
    print (asss)


if __name__ == '__main__':
    main()
