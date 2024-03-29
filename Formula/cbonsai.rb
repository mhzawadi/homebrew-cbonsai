class Cbonsai < Formula
  desc "Console Bonsai is a bonsai tree generator, written in C using ncurses"
  homepage "https://github.com/mhzawadi/homebrew-cbonsai"
  url "https://gitlab.com/jallbrit/cbonsai/-/archive/v1.3.1/cbonsai-v1.3.1.tar.gz"
  sha256 "62aa7e0eaf3098b7a6a2787146bd2531437df6ad0e604b0f9176128797efd8f9"
  license "GPL-3.0"
  depends_on "ncurses"
  depends_on "pkg-config"
  revision 1

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
