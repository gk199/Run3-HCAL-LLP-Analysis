from tensorflow.keras.models import Model
from tensorflow.keras.layers import (
	Activation,
	AveragePooling2D,
	GlobalAveragePooling2D,
	MaxPooling3D,    
	Conv2D,
	Conv3D,    
	Dense,
	Dropout,
	Flatten,
	Input,
	Reshape,
	UpSampling2D,
)
#from tensorflow.keras import backend as K
from tensorflow.keras.layers import concatenate
import tensorflow as tf
from tensorflow.keras.utils import register_keras_serializable
from tensorflow.keras.initializers import HeNormal

#from qkeras import QActivation, QConv2D, QDense, QDenseBatchnorm, quantized_bits

class Classifier2DCNN:
	def __init__(self):
		pass 

	def get_model(self):
		inputs = Input(shape=(10,10,1), name="classifier_inputs_")
		x = Conv2D(filters=74, kernel_size=(3, 3), strides=1, padding="same", activation="relu", name="conv1")(inputs)
		x = AveragePooling2D((2, 2), name="classifier_pool_1")(x) # not gradacm og
		x = Conv2D(filters=124, kernel_size=(3, 3), strides=1, padding="same", activation="relu", name="conv2")(x)
		x = Conv2D(filters=124, kernel_size=(3, 3), strides=1, padding="same", activation="relu", name="conv3")(x)   
		x = Conv2D(filters=124, kernel_size=(3, 3), strides=1, padding="same", activation="relu", name="lastconv")(x)  
		#x = GlobalAveragePooling2D(name="classifier_pool_1")(x)
		x = Flatten(name="classifier_flatten")(x)
		x = Dense(161, activation="sigmoid", name="dense1")(x) #Latent
		x = Dense(119, activation="sigmoid", name="dense2")(x)        
		output = Dense(2, activation="softmax", name="classifier_output")(x) #Output
		#print( inputs.shape, output.shape )
		return Model(inputs, output, name="classifier2d")

class Conv2DNN_Autoencoder:
	def __init__(self, input_shape: tuple):
		self.input_shape = input_shape

	def get_model(self):
		inputs = Input(shape=self.input_shape, name="classifier_inputs_")
		x = Reshape((10, 10, 1), name="classifier_reshape")(inputs)
		x = Conv2D(20, (3, 3), strides=1, padding="same", name="classifier_conv2d_1")(x)
		x = Activation("relu", name="classifier_relu_1")(x)
		x = AveragePooling2D((2, 2), name="classifier_pool_1")(x)
		x = Conv2D(30, (3, 3), strides=1, padding="same", name="classifier_conv2d_2")(x)
		x = Activation("relu", name="classifier_relu_2")(x)
		x = Flatten(name="classifier_flatten")(x)
		x = Dense(2, activation="relu", name="classifier_latent")(x) #Latent
		x = Dense(5 * 5 * 30, name="classifier_dense")(x)
		x = Reshape((5, 5, 30), name="classifier_reshape2")(x)
		x = Activation("relu", name="classifier_relu_3")(x)
		x = Conv2D(30, (3, 3), strides=1, padding="same", name="classifier_conv2d_3")(x)
		x = Activation("relu", name="classifier_relu_4")(x)
		x = UpSampling2D((2, 2), name="classifier_upsampling")(x)
		x = Conv2D(20, (3, 3), strides=1, padding="same", name="classifier_conv2d_4")(x)
		x = Activation("relu", name="classifier_relu_5")(x)
		outputs = Conv2D(
			1,
			(3, 3),
			activation="relu",
			strides=1,
			padding="same",
			name="classifier_outputs",
		)(x)
		#print( inputs.shape, outputs.shape )
		return Model(inputs, outputs, name="classifier")

class TeacherAutoencoder:
	def __init__(self, input_shape: tuple):
		self.input_shape = input_shape

	def get_model(self):
		inputs = Input(shape=self.input_shape, name="teacher_inputs_")
		x = Reshape((10, 10, 1), name="teacher_reshape")(inputs)
		x = Conv2D(20, (3, 3), strides=1, padding="same", name="teacher_conv2d_1")(x)
		x = Activation("relu", name="teacher_relu_1")(x)
		x = AveragePooling2D((2, 2), name="teacher_pool_1")(x)
		x = Conv2D(10, (3, 3), strides=1, padding="same", name="teacher_conv2d_2")(x)
		x = Activation("relu", name="teacher_relu_2")(x)
		x = Flatten(name="teacher_flatten")(x)
		x = Dense(2, activation="relu", name="teacher_latent")(x) #Latent
		x = Dense(5 * 5 * 30, name="teacher_dense")(x)
		x = Reshape((5, 5, 30), name="teacher_reshape2")(x)
		x = Activation("relu", name="teacher_relu_3")(x)
		x = Conv2D(10, (3, 3), strides=1, padding="same", name="teacher_conv2d_3")(x)
		x = Activation("relu", name="teacher_relu_4")(x)
		x = UpSampling2D((2, 2), name="teacher_upsampling")(x)
		x = Conv2D(10, (3, 3), strides=1, padding="same", name="teacher_conv2d_4")(x)
		x = Activation("relu", name="teacher_relu_5")(x)
		outputs = Conv2D(
			1,
			(3, 3),
			activation="relu",
			strides=1,
			padding="same",
			name="teacher_outputs",
		)(x)
		#print( inputs.shape, outputs.shape )
		return Model(inputs, outputs, name="teacher")


class CicadaV1:
	def __init__(self, input_shape: tuple):
		self.input_shape = input_shape

	def get_model(self):
		inputs = Input(shape=self.input_shape, name="inputs_")
		x = QDenseBatchnorm(
			16,
			kernel_quantizer=quantized_bits(8, 1, 1, alpha=1.0),
			bias_quantizer=quantized_bits(8, 3, 1, alpha=1.0),
			name="dense1",
		)(inputs)
		x = QActivation("quantized_relu(10, 6)", name="relu1")(x)
		x = Dropout(1 / 8)(x)
		x = QDense(
			1,
			kernel_quantizer=quantized_bits(12, 3, 1, alpha=1.0),
			use_bias=False,
			name="dense2",
		)(x)
		outputs = QActivation("quantized_relu(16, 8)", name="outputs")(x)
		return Model(inputs, outputs, name="cicada-v1")


class CicadaV2:
	def __init__(self, input_shape: tuple):
		self.input_shape = input_shape

	def get_model(self):
		inputs = Input(shape=self.input_shape, name="inputs_")
		x = Reshape((18, 14, 1), name="reshape")(inputs)
		x = QConv2D(
			4,
			(2, 2),
			strides=2,
			padding="valid",
			use_bias=False,
			kernel_quantizer=quantized_bits(12, 3, 1, alpha=1.0),
			name="conv",
		)(x)
		x = QActivation("quantized_relu(10, 6)", name="relu0")(x)
		x = Flatten(name="flatten")(x)
		x = Dropout(1 / 9)(x)
		x = QDenseBatchnorm(
			16,
			kernel_quantizer=quantized_bits(8, 1, 1, alpha=1.0),
			bias_quantizer=quantized_bits(8, 3, 1, alpha=1.0),
			name="dense1",
		)(x)
		x = QActivation("quantized_relu(10, 6)", name="relu1")(x)
		x = Dropout(1 / 8)(x)
		x = QDense(
			1,
			kernel_quantizer=quantized_bits(12, 3, 1, alpha=1.0),
			use_bias=False,
			name="dense2",
		)(x)
		outputs = QActivation("quantized_relu(16, 8)", name="outputs")(x)
		return Model(inputs, outputs, name="cicada-v2")
    
@register_keras_serializable()
class CustomLoss(tf.keras.losses.Loss):
	def __init__(self, reduction=tf.keras.losses.Reduction.AUTO, name="custom_loss"):
		super(CustomLoss, self).__init__(reduction=reduction, name=name)
		self.model = None
		self.X_anomaly = None

	def call(self, y_true, y_pred):
		if self.model is None:
			raise ValueError("Bad")
		loss1 = tf.reduce_mean(tf.square(y_true - y_pred))
		y_anomaly_pred = self.model(self.X_anomaly, training=False)
		loss2 = tf.reduce_mean(tf.square(self.X_anomaly - y_anomaly_pred))
		return loss1 + self.loss_weight * loss2
    
	def set_params(self, model, X_anomaly, loss_weight):
		self.model = model
		self.X_anomaly = X_anomaly["HToSSTobbbb_MH-350_MS-160_CTau10000train"]
		self.loss_weight = loss_weight 
        
        
class Classifier3DCNN:
	def __init__(self):
		pass #no need to specify input shape

	def get_model(self):
		inputs = Input(shape=(10, 10, 4, 1), name="classifier_inputs_")
		x = Conv3D(kernel_size=(3, 3, 3), filters=95, strides=1, padding="same", data_format="channels_last", activation="relu")(inputs)    
		#x = MaxPooling3D((3,3,3))(x) #for gradCAm no
		x = Conv3D(filters=95, kernel_size=(3, 3, 3), strides=1, padding="same", data_format="channels_last", activation="relu", kernel_initializer=HeNormal(seed=42))(x)
		x = Conv3D(filters=95, kernel_size=(3, 3, 3), strides=1, padding="same", data_format="channels_last", activation="relu", kernel_initializer=HeNormal(seed=42))(x)
		x = Conv3D(filters=95, kernel_size=(2, 2, 2), strides=1, padding="same", data_format="channels_last", activation="relu", kernel_initializer=HeNormal(seed=42))(x)
		x = Conv3D(filters=95, kernel_size=(3, 3, 1), strides=1, padding="same", data_format="channels_last", activation="relu", kernel_initializer=HeNormal(seed=42))(x)        
		x = Conv3D(filters=95, kernel_size=(3, 3, 3), strides=1, padding="same", data_format="channels_last", activation="relu", kernel_initializer=HeNormal(seed=42), name="lastconv")(x) 
		x = MaxPooling3D((3,3,1))(x)
		x = Flatten(name="classifier_flatten")(x)
		x = Dense(88, activation="sigmoid")(x) #Latent
		x = Dense(220, activation="sigmoid")(x)
		output = Dense(2, activation="softmax", name="classifier_output")(x) #Output
		return Model(inputs, output, name="classifier3d")
    
class Classifier2DGrad:
	def __init__(self):
		pass
	def get_model(self):
		inputs = Input(shape=(10,10,1), name="classifier_inputs_")
		x = Conv2D(3, (3, 3), padding='same', name="conv")(inputs)
		x = GlobalAveragePooling2D()(x)
		x = Dense(128, activation='relu')(x)
		x = Dense(128, activation='relu')(x)
		x = Dense(128, activation='relu')(x)
		output = Dense(2, activation="softmax", name="classifier_output")(x)
		return Model(inputs, output, name="classifier2dgrad")
    
    
class MultiInput:
	def __init__(self, num_features=2):
		self.num_features = num_features
    
	def get_model(self):
		img_inputs = Input(shape=(10, 10, 4, 1))
		num_inputs = Input(shape=(self.num_features,))

		x_conv = Conv3D(filters=16, kernel_size=(3, 3, 3), strides=1, padding="same", data_format="channels_last", activation="relu")(img_inputs)
		x_conv = Conv3D(filters=95, kernel_size=(3, 3, 3), strides=1, padding="same", data_format="channels_last", activation="relu")(x_conv)
		x_conv = Conv3D(filters=95, kernel_size=(3, 3, 3), strides=1, padding="same", data_format="channels_last", activation="relu")(x_conv)
		x_conv = MaxPooling3D((3,3,3))(x_conv)
		x_conv = Flatten()(x_conv)
		x_conv = Model(inputs=img_inputs, outputs=x_conv)

		x_den = Dense(64, activation="sigmoid")(num_inputs)
		x_den = Dense(64, activation="sigmoid")(num_inputs)
		x_den = Flatten()(x_den)
		x_den = Model(inputs=num_inputs, outputs=x_den)

		combined = concatenate([x_conv.output, x_den.output])

		z = Dense(64, activation="relu")(combined)
		z = Dense(2, activation="softmax")(z)

		return Model(inputs=[img_inputs, num_inputs], outputs=z)
