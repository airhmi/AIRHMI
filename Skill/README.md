# airhmi-dev — AIRHMI skill for AI coding agents

`airhmi-dev-<version>.zip` in this folder is the **airhmi-dev** skill: it teaches an AI coding agent (Claude Code,
Codex, Cursor, Gemini CLI, GitHub Copilot …) how to build AIRHMI touch-screen projects with the installed
**AIRHMI Designer**: scenario → mockup → `.airhmi.json` → event code → export → simulator → device.

**Requirements:** Windows 10/11, AIRHMI Designer 3.5 or newer (free: https://www.airhmi.com), an agent that runs on
your PC and reads `SKILL.md` skills. Cloud chats (claude.ai, ChatGPT) cannot reach the Designer on your PC.

## Kurulum (Türkçe)

**En kolayı: ajanınıza kurdurun.** Claude Code'a (ya da SKILL.md okuyan başka bir ajana) şunu yazın:

> airhmi-dev skill'ini kur: https://github.com/airhmi/AIRHMI/tree/main/Skill

Ajan bu klasördeki en son zip'i indirip kendi skills klasörüne açar. İndirme ve kopyalama izni sorarsa onaylayın.
Skill görünmezse yeni bir oturum açın.

- Claude Code: `%USERPROFILE%\.claude\skills\airhmi-dev\`
- Codex, Cursor, Gemini CLI, GitHub Copilot: `%USERPROFILE%\.agents\skills\airhmi-dev\`
- `SKILL.md` doğrudan bu klasörün içinde olmalı (`airhmi-dev\airhmi-dev\SKILL.md` değil).

**Elle kurulum:** zip'i indirin, yukarıdaki klasöre açın.
**Güncelleme:** "airhmi-dev skill'ini GitHub'daki son sürüme güncelle" deyin.
**Kontrol:** "airhmi-dev skill'i yüklü mü? Ortam kontrolünü çalıştır" deyin.

## Install (English)

**Easiest: let your agent install it.** Paste this into Claude Code (or another agent that reads `SKILL.md`):

> Install the airhmi-dev skill from https://github.com/airhmi/AIRHMI/tree/main/Skill

The agent downloads the latest zip from this folder and extracts it into its skills folder. Approve the download and
copy steps when it asks. If the skill is not listed afterwards, start a new session.

- Claude Code: `%USERPROFILE%\.claude\skills\airhmi-dev\`
- Codex, Cursor, Gemini CLI, GitHub Copilot: `%USERPROFILE%\.agents\skills\airhmi-dev\`
- `SKILL.md` must be directly inside that folder (not `airhmi-dev\airhmi-dev\SKILL.md`).

**Manual install:** download the zip and extract it into the folder above.
**Update:** say *"Update the airhmi-dev skill to the latest version from https://github.com/airhmi/AIRHMI/tree/main/Skill"*.
**Check:** say *"Is the airhmi-dev skill installed? Run its environment check."*

The full guide is `README.md` inside the zip.
