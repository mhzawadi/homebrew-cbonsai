class Cbonsai < Formula
  desc "Console Bonsai is a bonsai tree generator, written in C using ncurses"
  homepage "https://github.com/mhzawadi/homebrew-cbonsai"
  url "https://github.com/mhzawadi/homebrew-cbonsai/archive/refs/tags/v1.3.2.tar.gz"
  sha256 "79139324a9af73aa4c692398f70f156c1cd4ab0bf2660f927907396ad4247f52"
  license "GPL-3.0"
  depends_on "ncurses"
  depends_on "pkg-config"

  def install
    ENV["PKG_CONFIG_PATH"] = "$PKG_CONFIG_PATH:/usr/local/Cellar/ncurses/6.2/lib/pkgconfig/:/opt/homebrew/Cellar/ncurses/6.2/lib/pkgconfig/"
    system "make"
    bin.install "cbonsai"
  end

  test do
    system "~/.local/bin/cbonsai", "-p"
  end
end
