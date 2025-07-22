# Picoshell

1. Loop por todos os comandos
2. pipe creation
    2.1 Verificar se exist um próximo comando na lista
    2.2 se sim criar o pipe (!gerenciar erro)
3. criação do fork (!gerenciar erro)
Child process
4. Verificar se o prev_fd é diferente de -1
    4.1 se for setar ele como STDINT e fechalo apos isso (lidar com erro)
5. verificar se existe um próximo comando
    5.1 fechar o fd de leitura
    5.2 setar o de saida para o pipe (lidar com erros)
    5.3 execvp, (com exit para lidar com erro)
Parent process
6. verificar se o prev_fd foi utilizado, caso sim fechar ele
7. verificar se teve próximo comando e se tiver fechar o fd para write e colocar o prev_fd como ele
