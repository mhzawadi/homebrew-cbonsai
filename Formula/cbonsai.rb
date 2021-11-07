class Cbonsai < Formula
  desc "Console Bonsai is a bonsai tree generator, written in C using ncurses"
  homepage "https://github.com/mhzawadi/homebrew-cbonsai"
  url "https://github.com/mhzawadi/homebrew-cbonsai/archive/refs/tags/v1.3.3.tar.gz"
  sha256 "0eb83c279690b482cdaf19f8eda1d302f52cf5038c942b04625adeca90454797"
  license "GPL-3.0"
  depends_on "ncurses"
  depends_on "pkg-config"

  def install
    ENV["PKG_CONFIG_PATH"] = "$PKG_CONFIG_PATH:/usr/local/Cellar/ncurses/6.2/lib/pkgconfig/"
    ENV.append_path "PKG_CONFIG_PATH", "/opt/homebrew/Cellar/ncurses/6.2/lib/pkgconfig/"
    system "make"
    bin.install "cbonsai"
  end

  test do
    system "~/.local/bin/cbonsai", "-p"
  end
end
