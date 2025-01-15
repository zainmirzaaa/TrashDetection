import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv2D, MaxPooling2D, Flatten, Dense
from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras.preprocessing import image
import numpy as np
import random
import json
import time

# Simulate drone positions over a grid
def generate_drone_positions(rows=10, cols=10):
    positions = []
    for x in range(rows):
        for y in range(cols):
            positions.append({"x": x, "y": y})
    random.shuffle(positions)
    return positions

# Save drone positions to a file
def save_positions(positions, filename='drone_positions.json'):
    with open(filename, 'w') as f:
        json.dump(positions, f)

# Generate and save positions
positions = generate_drone_positions()
save_positions(positions)
print(f"Saved {len(positions)} drone positions to drone_positions.json")

# Simulate drone capturing images (here we just simulate predictions)
def simulate_trash_detection(positions):
    detections = []
    for pos in positions:
        trash_present = random.random() > 0.5
        category = random.choice(['plastic','metal','organic']) if trash_present else None
        detections.append({"x": pos['x'], "y": pos['y'], "trashDetected": trash_present, "category": category})
    return detections


def preprocess_image(img_path):
    """Load and preprocess an image for prediction"""
    img = image.load_img(img_path, target_size=(64,64))
    img_array = image.img_to_array(img) / 255.0
    img_array = np.expand_dims(img_array, axis=0)
    return img_array


# Create a CNN model
model = Sequential([
    Conv2D(32, (3,3), activation='relu', input_shape=(64,64,3)),
    MaxPooling2D(2,2),
    Conv2D(64, (3,3), activation='relu'),
    MaxPooling2D(2,2),
    Flatten(),
    Dense(128, activation='relu'),
    Dense(3, activation='softmax')  # Assume 3 categories of trash
])

model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

# Data generators
train_gen = ImageDataGenerator(rescale=1./255).flow_from_directory(
    'data/train', target_size=(64,64), batch_size=32, class_mode='categorical')

val_gen = ImageDataGenerator(rescale=1./255).flow_from_directory(
    'data/val', target_size=(64,64), batch_size=32, class_mode='categorical')

# Train model
model.fit(train_gen, validation_data=val_gen, epochs=10)

# Save model
model.save('trash_detector_model.h5')

# Load the trained model for predictions
def load_trash_model(path='trash_detector_model.h5'):
    """Load the saved CNN model"""
    loaded_model = tf.keras.models.load_model(path)
    return loaded_model

model = load_trash_model()
print("Model loaded for inference.")

def predict_trash(img_path):
    """Predict trash category for a given image"""
    img_array = preprocess_image(img_path)
    prediction = model.predict(img_array)
    categories = ['plastic', 'metal', 'organic']
    predicted_class = categories[np.argmax(prediction)]
    return predicted_class

