class ZellnoComboLockMenu : UIScriptedMenu
{
    protected static ref ZellnoComboLockMenu s_ActiveMenu;
    protected static Zellno_CombinationLock4 s_PendingLock;

    protected Zellno_CombinationLock4 m_Lock;
    protected TextWidget m_TitleText;
    protected TextWidget m_InstructionText;
    protected PasswordEditBoxWidget m_CodeInput;
    protected TextWidget m_StatusText;
    protected ButtonWidget m_UnlockButton;
    protected ButtonWidget m_CancelButton;
    protected bool m_IsSetup;
    protected bool m_InputCaptured;
    protected bool m_CloseRequested;
    protected bool m_AdjustingInput;
    protected string m_LastObservedInput;

    protected void ApplyScreenGeometry()
    {
        if (!layoutRoot)
            return;

        int screenWidth;
        int screenHeight;
        GetScreenSize(screenWidth, screenHeight);

        float windowWidth = 500;
        float windowHeight = 320;
        float windowX = (screenWidth - windowWidth) * 0.5;
        float windowY = (screenHeight - windowHeight) * 0.5;

        layoutRoot.ClearFlags(WidgetFlags.IGNOREPOINTER, false);
        layoutRoot.SetFlags(WidgetFlags.VISIBLE | WidgetFlags.RENDER_ALWAYS, false);
        layoutRoot.SetScreenPos(0, 0, false);
        layoutRoot.SetScreenSize(screenWidth, screenHeight, false);

        Widget dimWidget = layoutRoot.FindAnyWidget("Dim");
        if (dimWidget)
        {
            dimWidget.SetFlags(WidgetFlags.VISIBLE | WidgetFlags.RENDER_ALWAYS, false);
            dimWidget.SetScreenPos(0, 0, false);
            dimWidget.SetScreenSize(screenWidth, screenHeight, false);
        }

        Widget windowWidget = layoutRoot.FindAnyWidget("Window");
        if (windowWidget)
        {
            windowWidget.SetFlags(WidgetFlags.VISIBLE | WidgetFlags.RENDER_ALWAYS, false);
            windowWidget.SetScreenPos(windowX, windowY, false);
            windowWidget.SetScreenSize(windowWidth, windowHeight, false);
        }

        layoutRoot.Show(true, false);
        layoutRoot.Update();
    }

    static void ScheduleUnlockMenu(Zellno_CombinationLock4 lock)
    {
        s_PendingLock = lock;
        g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(OpenPendingUnlockMenu, 250, false);
    }

    protected static void OpenPendingUnlockMenu()
    {
        Zellno_CombinationLock4 lock = s_PendingLock;
        s_PendingLock = null;

        if (lock)
            OpenForLock(lock, false);
    }

    static void OpenForLock(Zellno_CombinationLock4 lock, bool isSetup)
    {
        if (!lock || !g_Game || !g_Game.IsClient())
            return;

        if (s_ActiveMenu)
            s_ActiveMenu.RequestClose("substituido");

        ZellnoComboLockMenu menu = new ZellnoComboLockMenu;
        menu.SetLock(lock);
        menu.m_IsSetup = isSetup;
        s_ActiveMenu = menu;
        g_Game.GetUIManager().ShowScriptedMenu(menu, null);
    }

    static void HandleServerResult(Zellno_CombinationLock4 lock, int result)
    {
        if (result == Zellno_CombinationLock4.ZELLNO_RESULT_ACCEPTED)
        {
            if (s_PendingLock == lock)
                s_PendingLock = null;

            if (s_ActiveMenu && s_ActiveMenu.m_Lock == lock)
                s_ActiveMenu.RequestClose("servidor aceitou");

            return;
        }

        if (!s_ActiveMenu || s_ActiveMenu.m_Lock != lock)
            return;

        if (result == Zellno_CombinationLock4.ZELLNO_RESULT_WAIT)
        {
            s_ActiveMenu.SetStatus("Please wait before trying again.");
        }
        else
        {
            s_ActiveMenu.SetStatus("Incorrect password.");
            s_ActiveMenu.ClearCode();
        }
    }

    void SetLock(Zellno_CombinationLock4 lock)
    {
        m_Lock = lock;
    }

    override Widget Init()
    {
        layoutRoot = g_Game.GetWorkspace().CreateWidgets("ZellnoComboLock/gui/layouts/zellno_combo_lock.layout");

        if (!layoutRoot)
            return null;

        ApplyScreenGeometry();

        m_TitleText = TextWidget.Cast(layoutRoot.FindAnyWidget("Title"));
        m_InstructionText = TextWidget.Cast(layoutRoot.FindAnyWidget("Instruction"));
        m_CodeInput = PasswordEditBoxWidget.Cast(layoutRoot.FindAnyWidget("CodeInput"));
        m_StatusText = TextWidget.Cast(layoutRoot.FindAnyWidget("StatusText"));
        m_UnlockButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("UnlockButton"));
        m_CancelButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("CancelButton"));

        if (m_TitleText)
            m_TitleText.SetTextExactSize(28);

        if (m_InstructionText)
            m_InstructionText.SetTextExactSize(20);

        if (m_StatusText)
            m_StatusText.SetTextExactSize(18);

        if (m_UnlockButton)
            m_UnlockButton.SetTextProportion(0.42);

        if (m_CancelButton)
            m_CancelButton.SetTextProportion(0.42);

        if (m_CodeInput)
        {
            m_CodeInput.SetHideText(false);
            m_LastObservedInput = "";
            SetFocus(m_CodeInput);
        }

        if (m_InstructionText && m_IsSetup)
            m_InstructionText.SetText("Create a 4-6 digit password");
        else if (m_InstructionText)
            m_InstructionText.SetText("Enter password");

        if (m_UnlockButton && m_IsSetup)
            m_UnlockButton.SetText("SET PASSWORD");
        else if (m_UnlockButton)
            m_UnlockButton.SetText("UNLOCK");

        if (m_CancelButton)
            m_CancelButton.SetText("CANCEL");

        layoutRoot.Show(true);

        return layoutRoot;
    }

    override void Cleanup()
    {
        ReleaseInputMode("Cleanup");

        if (s_ActiveMenu == this)
            s_ActiveMenu = null;

        super.Cleanup();
    }

    override void OnShow()
    {
        super.OnShow();
        ApplyScreenGeometry();
        g_Game.GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
        m_InputCaptured = true;

        Hud hud = g_Game.GetMission().GetHud();
        if (hud)
        {
            hud.ShowQuickbarUI(false);
            hud.ShowHudUI(false);
        }
    }

    override void OnHide()
    {
        ReleaseInputMode("OnHide");
        super.OnHide();
    }

    protected void SetStatus(string text)
    {
        if (m_StatusText)
            m_StatusText.SetText(text);
    }

    protected void ReleaseInputMode(string origin)
    {
        if (!m_InputCaptured)
            return;

        m_InputCaptured = false;

        if (!g_Game || !g_Game.GetMission())
            return;

        Hud hud = g_Game.GetMission().GetHud();
        if (hud)
        {
            hud.ShowQuickbarUI(true);
            hud.ShowHudUI(true);
        }

        g_Game.GetMission().PlayerControlEnable(true);
    }

    protected void RequestClose(string origin)
    {
        if (m_CloseRequested)
            return;

        m_CloseRequested = true;
        SetFocus(null);
        Close();
    }

    protected void ClearCode()
    {
        if (m_CodeInput)
        {
            SetFocus(null);
            m_CodeInput.SetText("");
            m_CodeInput.Update();
            m_LastObservedInput = "";
            SetFocus(m_CodeInput);
        }
    }

    protected void Submit()
    {
        if (!m_Lock || !m_CodeInput)
        {
            RequestClose("submit invalido");
            return;
        }

        string code = m_CodeInput.GetText();

        int codeLength = code.Length();
        if (codeLength < 4 || codeLength > 6)
        {
            SetStatus("Enter 4 to 6 digits.");
            return;
        }

        for (int i = 0; i < codeLength; ++i)
        {
            if ("0123456789".IndexOf(code.Get(i)) == -1)
            {
                SetStatus("Use digits from 0 to 9 only.");
                return;
            }
        }

        SetStatus("Validating...");
        PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
        if (!player)
        {
            RequestClose("jogador ausente");
            return;
        }

        g_Game.RPCSingleParam(player, Zellno_CombinationLock4.ZELLNO_RPC_SUBMIT_CODE, new Param2<Object, string>(m_Lock, code), true);
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (w == m_UnlockButton)
        {
            Submit();
            return true;
        }

        if (w == m_CancelButton)
        {
            RequestClose("botao cancelar");
            return true;
        }

        return super.OnClick(w, x, y, button);
    }

    override bool OnKeyDown(Widget w, int x, int y, int key)
    {
        if (key == KeyCode.KC_ESCAPE)
        {
            RequestClose("tecla Escape");
            return true;
        }

        // Backspace e Delete ficam deliberadamente sob controle do EditBox nativo.
        // O fallback global de Escape e consultado em Update(), como nos menus vanilla.
        return super.OnKeyDown(w, x, y, key);
    }

    override bool OnChange(Widget w, int x, int y, bool finished)
    {
        if (w != m_CodeInput || m_AdjustingInput)
            return super.OnChange(w, x, y, finished);

        NormalizeCodeInput();
        return true;
    }

    protected void NormalizeCodeInput()
    {
        if (!m_CodeInput || m_AdjustingInput)
            return;

        string raw = m_CodeInput.GetText();
        string filtered = "";

        for (int i = 0; i < raw.Length() && filtered.Length() < 6; ++i)
        {
            string character = raw.Get(i);
            if ("0123456789".IndexOf(character) != -1)
                filtered = filtered + character;
        }

        if (filtered != raw)
        {
            m_AdjustingInput = true;
            m_CodeInput.SetText(filtered);
            m_CodeInput.Update();
            m_AdjustingInput = false;
            SetFocus(m_CodeInput);
        }

        if (raw != m_LastObservedInput && m_StatusText)
            m_StatusText.SetText("");

        m_LastObservedInput = filtered;
    }

    override void Update(float timeslice)
    {
        super.Update(timeslice);

        // Com o foco no EditBox, o widget consome eventos brutos de teclado e
        // OnKeyDown pode nao receber KC_ESCAPE. Os menus vanilla consultam a
        // acao global de retorno por quadro para fechar de forma confiavel.
        if (g_Game && GetUApi().GetInputByID(UAUIBack).LocalPress())
        {
            RequestClose("acao UAUIBack");
            return;
        }

        NormalizeCodeInput();
    }

}
