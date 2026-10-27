# Внесение вклада

<p align="center">
  <a href="CONTRIBUTING.md">English</a> |
  <a href="CONTRIBUTING.zh-CN.md">简体中文</a> |
  <a href="CONTRIBUTING.de-DE.md">Deutsch</a> |
  <a href="CONTRIBUTING.fr-FR.md">Français</a> |
  <a href="CONTRIBUTING.es-ES.md">Español</a> |
  <a href="CONTRIBUTING.ko-KR.md">한국어</a> |
  <span>Русский</span>
</p>

## Процесс

1. Создайте issue с описанием изменений, которые вы хотите внести. Если речь идёт лишь о небольших изменениях или исправлениях ошибок, можно перейти сразу к шагу 3.
2. После обсуждения объёма работ в issue назначьте его на себя. Оно должно появиться в колонке "To do" проекта OpenBot.
3. Сделайте форк проекта и клонируйте его локально:

   `git clone https://github.com/<user_id>/OpenBot.git`

4. Создайте ветку:

   `git checkout -b <branch-name>`

   где `<branch-name>` кратко описывает объём работы.

5. Выполните работу, пишите осмысленные сообщения коммитов и отправьте ветку в форкнутый репозиторий:

   ```bash
   git add <modified file>
   git commit -m <meaningful description>
   git push --set-upstream origin <branch-name>
   ```

6. Создайте [pull request](https://github.com/ob-f/OpenBot/pulls) на GitHub и свяжите с ним issue. Он должен появиться в колонке "In progress" проекта OpenBot.
7. Обработайте возможные замечания из код-ревью и отправьте их в свой форк. Pull request обновляется автоматически.
8. Возьмите холодный напиток по вашему вкусу, чтобы вознаградить себя за то, что делаете мир лучше.

## Рекомендации

- Используйте тот же стиль и форматирование, что и в остальном коде.
  - Для кода на Java (Android) и Python см. [ниже](#форматирование).
  - Для любого другого кода просто старайтесь придерживаться общего стиля.
- Обновляйте документацию, связанную с внесёнными вами изменениями в коде.
- Если вы хотите добавить сторонние зависимости, пожалуйста, сначала обсудите это в issue.
- Pull request должен реализовывать одну функцию с минимальным количеством изменений.
- Убедитесь, что вы не добавляете временные или бинарные файлы (файлы .gitignore должны большую часть этого брать на себя).
- Перед отправкой pull request выполните rebase/merge master в вашу ветку.
- По возможности протестируйте ваш код на Windows, Linux и OSX.

## Форматирование

### Java

Мы используем gradle-скрипт для форматирования кода на Java. Убедитесь, что вы находитесь в каталоге `robot/android`.

Проверить ваш код можно командой:

```bash
./gradlew checkStyle
```

Применить стиль ко всем файлам можно командой:

```bash
./gradlew applyStyle
```

### Python

Для форматирования кода на Python мы используем [black](https://pypi.org/project/black/).

Проверить код в текущем каталоге можно командой:

```bash
black --check .
```

Применить стиль ко всем файлам в текущем каталоге можно командой:

```bash
black .
```

## Дополнительные ресурсы

Если вы ищете больше информации об участии в open-source проектах, вот два полезных справочника:

- [How to Contribute to Open Source](http://opensource.guide/how-to-contribute/)
- [The beginner's guide to contributing to a GitHub project](https://akrabat.com/the-beginners-guide-to-contributing-to-a-github-project/)

Большое спасибо!
