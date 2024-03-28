# Algorithmic Composer

> A Tool for Algorithmic Composition

[![Two-dimensional Algorithmics](https://arts.adelaide.edu.au/sites/default/files/styles/ua_image/public/2018-12/Music%20Composition.jpg?itok=MnnrHbju)](https://alandefreitas.github.io/composer/)

<br/>

> Work in progress

<br/>

[![Build Status](https://img.shields.io/github/workflow/status/alandefreitas/composer/Build?event=push&label=Build&logo=Github-Actions)](https://github.com/alandefreitas/composer/actions?query=workflow%3ABuild+event%3Apush)
[![Latest Release](https://img.shields.io/github/release/alandefreitas/composer.svg?label=Download)](https://GitHub.com/alandefreitas/composer/releases/)
[![Website lbesson.bitbucket.org](https://img.shields.io/website-up-down-green-red/http/alandefreitas.github.io/composer.svg?label=Documentation)](https://alandefreitas.github.io/composer/)

<br/>



<!-- https://github.com/bradvin/social-share-urls -->
[![Facebook](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+Facebook&logo=facebook)](https://www.facebook.com/sharer/sharer.php?t=Algorithmic%20Composition%20C%2B%2B&u=https://github.com/alandefreitas/composer/)
[![QZone](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+QZone&logo=qzone)](http://sns.qzone.qq.com/cgi-bin/qzshare/cgi_qzshare_onekey?url=https://github.com/alandefreitas/composer/&title=Algorithmic%20Composition%20C%2B%2B&summary=Algorithmic%20Composition%20C%2B%2B)
[![Weibo](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+Weibo&logo=sina-weibo)](http://sns.qzone.qq.com/cgi-bin/qzshare/cgi_qzshare_onekey?url=https://github.com/alandefreitas/composer/&title=Algorithmic%20Composition%20C%2B%2B&summary=Algorithmic%20Composition%20C%2B%2B)
[![Reddit](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+Reddit&logo=reddit)](http://www.reddit.com/submit?url=https://github.com/alandefreitas/composer/&title=Algorithmic%20Composition%20C%2B%2B)
[![Twitter](https://img.shields.io/twitter/url/http/shields.io.svg?label=Share+on+Twitter&style=social)](https://twitter.com/intent/tweet?text=Algorithmic%20Composition%20C%2B%2B&url=https://github.com/alandefreitas/composer/&hashtags=MOO,MultiObjectiveComposition,Cpp,ScientificComputing,Composition,Developers)
[![LinkedIn](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+LinkedIn&logo=linkedin)](https://www.linkedin.com/shareArticle?mini=false&url=https://github.com/alandefreitas/composer/&title=Algorithmic%20Composition%20C%2B%2B)
[![WhatsApp](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+WhatsApp&logo=whatsapp)](https://api.whatsapp.com/send?text=Algorithmic%20Composition%20C%2B%2B:+https://github.com/alandefreitas/composer/)
[![Line.me](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+Line.me&logo=line)](https://lineit.line.me/share/ui?url=https://github.com/alandefreitas/composer/&text=Algorithmic%20Composition%20C%2B%2B)
[![Telegram.me](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+Telegram.me&logo=telegram)](https://telegram.me/share/url?url=https://github.com/alandefreitas/composer/&text=Algorithmic%20Composition%20C%2B%2B)
[![HackerNews](https://img.shields.io/twitter/url/http/shields.io.svg?style=social&label=Share+on+HackerNews&logo=y-combinator)](https://news.ycombinator.com/submitlink?u=https://github.com/alandefreitas/composer/&t=Algorithmic%20Composition%20C%2B%2B)

<br/>

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->
<details>
<summary>Table of Contents</summary>

- [Quick start](#quick-start)
- [Algorithms](#algorithms)
- [Criteria](#criteria)
- [Integration](#integration)
  - [Packages](#packages)
  - [Build from source](#build-from-source)
  - [CMake targets](#cmake-targets)
- [Limitations](#limitations)
- [Contributing](#contributing)
- [Roadmap](#roadmap)
- [Thanks](#thanks)
- [References](#references)

</details>
<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Quick start

First, make sure you have the latest version of CMake in you system. Then, proceed with the following steps:

1. Clone the composerlib repository from GitHub using the following command:

```
git clone git@github.com:composernew/composerlib.git
```

2. Enter the composerlib directory:

```
cd composerlib
```

3. Create a directory to build the project:

```
mkdir build
cd build
```

4. Configure the build script

```
cmake .. -DCMAKE_C_COMPILER=/usr/bin/gcc-10 -DCMAKE_CXX_COMPILER=/usr/bin/g++-10  -DCMAKE_CXX_FLAGS="-O2" -DCMAKE_BUILD_TYPE=Release
```

5. Build all targets:

```
cmake --build . -j 2 --config Release
```

6. Start with the `optimizer` example:

```
cd examples
./example_optimizer
```

## Algorithms

## Criteria

## Integration

### Packages

### Build from source

#### Dependencies

#### Building the Examples

#### Installing from Source

#### Building the packages

### CMake targets

#### Find it as a CMake Package

#### Use it as a CMake subdirectory

#### CMake with Automatic Download

## Limitations

## Contributing

## Roadmap

## Thanks

## References
