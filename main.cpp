#include <windows.h>

// Definição dos endereços de memória da sua versão do jogo
// Usando o módulo hw.dll que descobrimos no Process Hacker
DWORD ObterModuloHardware() {
    return (DWORD)GetModuleHandleA("hw.dll");
}

// Loop principal do Cheat que roda em segundo plano dentro do jogo
DWORD WINAPI LoopAimbot(LPVOID lpParam) {
    // Aguarda o jogo carregar completamente a hw.dll na memória
    DWORD hwDLL = 0;
    while (!hwDLL) {
        hwDLL = ObterModuloHardware();
        Sleep(100);
    }

    // Endereços de Pitch (Vertical) e Yaw (Horizontal) baseados na hw.dll
    float* pitch = (float*)(hwDLL + 0x1102A0);
    float* yaw   = (float*)(hwDLL + 0x1102A4);

    // Loop contínuo: roda até você pressionar a tecla 'END' no teclado
    while (!GetAsyncKeyState(VK_END)) {
        
        // SE SEGURAR O BOTÃO ESQUERDO DO MOUSE (VK_LBUTTON)
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            // Força a mira a travar na posição reta/centro (Ângulos 0.0)
            if (pitch) *pitch = 0.0f;
            if (yaw)   *yaw   = 0.0f;
        }

        Sleep(1); // Evita que o cheat consuma 100% do seu processador
    }

    // Descarrega a DLL do jogo com segurança se pressionar END
    FreeLibraryAndExitThread((HMODULE)lpParam, 0);
    return 0;
}

// Ponto de entrada padrão exigido pelo Windows para qualquer arquivo .dll
BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved) {
    if (dwReason == DLL_PROCESS_ATTACH) {
        // Desabilita chamadas desnecessárias para otimizar a performance
        DisableThreadLibraryCalls(hModule);
        // Cria a linha de execução (thread) independente do cheat
        CreateThread(NULL, 0, LoopAimbot, hModule, 0, NULL);
    }
    return TRUE;
}
