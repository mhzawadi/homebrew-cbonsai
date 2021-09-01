class Cbonsai < Formula
  desc "Console Bonsai is a bonsai tree generator, written in C using ncurses"
  homepage "https://github.com/mhzawadi/homebrew-cbonsai"
  url "https://github.com/mhzawadi/homebrew-cbonsai/archive/refs/tags/v1.3.1.tar.gz"
  sha256 "0ccc4c0c65133cfd61ffc2403596a44437d60724034945d829702b6050da2c95"
  license "GPL-3.0"
  depends_on "pkg-config"
  depends_on "ncurses"

  def install
    ENV["PKG_CONFIG_PATH"] = "$PKG_CONFIG_PATH:/usr/local/Cellar/ncurses/6.2/lib/pkgconfig/"
    system "make"
    bin.install "cbonsai"
  end

  test do
    system "~/.local/bin/cbonsai", "-p"
  end
end
