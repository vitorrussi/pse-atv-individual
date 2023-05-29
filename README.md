# README

Solução para a atividade 1 da matéria PSE.

Para escolher o número de carros, lugares e passageiros, edite o arquivo `defines.h`.

Alguns casos deram Segmentation fault no início, o motivo ao certo não foi descoberto, até acredito ser algum erro no framework do FreeRTOS escolhido. O arquivo `segfault.txt` mostra uma lista não exaustiva de casos que foram testados. O script `test_all.py` fez todos esses testes.

O FreeRTOS utilizado foi o `https://github.com/FreeRTOS/FreeRTOS`, release 202212.01 `https://github.com/FreeRTOS/FreeRTOS/releases/tag/202212.01`

## Build container

`docker build -t pse-atv-1 .`

## Run and attach to container bash

### Using docker run

`docker run --rm -it pse-atv-1`

### Using docker compose

`docker compose run --rm main bash`

## Build and run

```bash
make
make run
```
