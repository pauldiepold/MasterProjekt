layers = [2, 3]
layer_sizes1 = [25]
layer_sizes2 = [30]
layer_sizes3 = [15]
batch_size = [64]
epochs = [10]

models = {
}

for params in itertools.product(*[layers, layer_sizes1, layer_sizes2, layer_sizes3, batch_size, epochs]):
    NAME = str(params[0]) + 'layers_'
    for i in range(params[0]):
        NAME = NAME + str(params[i + 1]) + 'nodes' + str(i + 1) + '_'
    NAME = NAME + str(params[4]) + 'batch_' + str(params[5]) + 'epochs_' + str(int(time.time()))
    # NAME = "{}-layers_{}-nodes_{}-nodes2_{}-batch_{}-epochs_{}".format(*params, int(time.time()))
    # print('Name: ' + NAME + '\n')


    # build the models[NAME]['model'] and train it
    models[NAME] = {}
    models[NAME]['model'] = tf.keras.Sequential(name=NAME)

    models[NAME]['model'].add(
        tf.keras.layers.Dense(params[1], input_shape=(19,), activation='relu'))  # relu is used for performance

    if 2 <= params[0]:
        models[NAME]['model'].add(tf.keras.layers.Dense(params[2], activation='relu'))  # relu is used for performance

    if 3 <= params[0]:
        models[NAME]['model'].add(tf.keras.layers.Dense(params[3], activation='relu'))  # relu is used for performance

    models[NAME]['model'].add(tf.keras.layers.Dense(NUM_CLASSES,
                                           activation='softmax'))  # softmax is used, because we only expect one class to occur per input

    print(models[NAME]['model'].summary())
    print('\n\n------------------------------\n')

for NAME, model in models.items():
    log_dir = "logs/fit/" + datetime.datetime.now().strftime("%Y%m%d-%H%M%S") + "-{}".format(NAME)
    tensorboard = TensorBoard(log_dir=log_dir)

    models[NAME]['model'].compile(loss='mse',
                         optimizer='adam',
                         metrics=['accuracy'])

    print('\n------------------------------\n')

    models[NAME]['history'] = models[NAME]['model'].fit(inputs_train,
                                     outputs_train,
                                     epochs=params[5],
                                     batch_size=params[4],
                                     validation_data=(inputs_validate, outputs_validate),
                                     callbacks=[tensorboard])
