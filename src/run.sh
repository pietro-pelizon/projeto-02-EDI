#!/bin/bash

EXEC="./ted"
BASE="../testes"
SAIDA="../saida"

declare -A TESTES

TESTES["0-flor-encaixotada"]="0-cai-dentro 1-cai-fora-1 2-cai-fora-2"
TESTES["2-coroa-flores"]="01-d-segs-antep 02-d-circs-antep 03-d-txts-antep 04-p-circs-antep 05-clone-0d 06-d-all-antep-1 07-d-all-antep-2 08-d-txts-antep-3 09-clone-1d 10-clone-2d"
TESTES["3-emblemas"]="d-circs-antep"

for GEO_NOME in "${!TESTES[@]}"; do
    echo "=============================="
    echo " Geo: $GEO_NOME"
    echo "=============================="

    GEO_PATH="$BASE/$GEO_NOME.geo"
    if [ ! -f "$GEO_PATH" ]; then
        echo "  [SKIP] $GEO_NOME.geo não encontrado"
        continue
    fi

    OUT="$SAIDA/$GEO_NOME"
    mkdir -p "$OUT"

    for QRY in ${TESTES[$GEO_NOME]}; do
        QRY_PATH="$BASE/$GEO_NOME/$QRY.qry"
        if [ ! -f "$QRY_PATH" ]; then
            echo "  [SKIP] $QRY.qry não encontrado"
            continue
        fi
        echo -n "  Rodando $QRY... "
        $EXEC -e "$BASE" -f "$GEO_NOME.geo" -o "$OUT" -v "$GEO_NOME-v.via" -q "$GEO_NOME/$QRY.qry" 2>/dev/null
        RET=$?
        if [ $RET -eq 0 ]; then
            echo "[OK]"
        else
            echo "FALHOU (exit code $RET)"
        fi
    done
    echo ""
done

echo "Testes concluídos. Saídas em $SAIDA/"