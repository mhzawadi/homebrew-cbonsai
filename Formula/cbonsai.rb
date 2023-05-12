class Cbonsai < Formula
  desc "Console Bonsai is a bonsai tree generator, written in C using ncurses"
  homepage "https://github.com/mhzawadi/homebrew-cbonsai"
  url "https://gitlab.com/jallbrit/cbonsai/-/archive/master/cbonsai-master.tar.gz"
  sha256 "3a0241c604f17f10d9e97d27526616904eeab8021551706ae4bc833278b26d36"
  license "GPL-3.0"
  depends_on "ncurses"
  depends_on "pkg-config"

  def install
    ENV["PKG_CONFIG_PATH"] = "$PKG_CONFIG_PATH:/usr/local/Cellar/ncurses/6.4/lib/pkgconfig/"
    ENV.append_path "PKG_CONFIG_PATH", "/opt/homebrew/Cellar/ncurses/6.4/lib/pkgconfig/"
    system "make"
    bin.install "cbonsai"
  end

  test do
    system bin/"cbonsai", "-p"
  end
end
